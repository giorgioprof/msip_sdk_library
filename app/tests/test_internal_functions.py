import unittest
from unittest.mock import patch, MagicMock, call
import json
from pydantic import ValidationError


from dapr.ext.grpc import InvokeMethodRequest, InvokeMethodResponse, App
from app.pubsub.models import FileData
import app.pubsub.internal_functions  # Import the module containing the decorated function

class TestInspectFile(unittest.TestCase):
    
    def setUp(self):
        # Setup test data
        self.valid_data = {
            "file": "/test/path/file.docx",
            "application_id": "test-app-id-123"
        }

        self.invalid_data = {
            "file": "/test/path/file.docx",
            "wrong_id": "test-app-id-123"
        }
        
        # Mock request with valid data
        self.valid_request = MagicMock(spec=InvokeMethodRequest)
        self.valid_request.text.return_value = json.dumps(self.valid_data)
        self.valid_request.metadata = {"Content-Type": "application/json"}

        # Mock request with invalid data
        self.invalid_request = MagicMock(spec=InvokeMethodRequest)
        self.invalid_request.text.return_value = json.dumps(self.invalid_data)
        self.invalid_request.metadata = {"Content-Type": "application/json"}
        
        # Mock file status result
        self.file_status_result = dict(
            status=True,
            path="/test/path/file.docx",
            error=""
        )
        
        # Get the original inspect_file function (before decoration)
        # This is a way to get the original function without the decorator effects
        # Note: This approach may need to be adjusted based on how the app is structured
        self.original_inspect_file = None
        
        # Get the module where inspect_file is defined
        self.main_module = app.pubsub.internal_functions
        
        # Access the decorators and original functions
        # This approach depends on the app structure and may need adjustment
        self.inspect_file_func = self.main_module.inspect_file
        
        # Create a mock App for testing
        self.mock_app = MagicMock(spec=App)

    @patch('app.pubsub.internal_functions.instrumented_ext_get_file_status')
    @patch('app.pubsub.internal_functions.metrics_active_requests')
    @patch('app.pubsub.internal_functions.metrics_req_count')
    @patch('app.pubsub.internal_functions.metrics_req_latency')
    def test_inspect_file_success(self, mock_req_latency, mock_req_count, 
                                 mock_active_requests, mock_get_file_status):
        """Test successful file inspection by directly calling the function"""
        # Setup mocks
        mock_get_file_status.return_value = self.file_status_result
        mock_active_requests.labels.return_value = MagicMock()
        mock_req_count.labels.return_value = MagicMock()
        mock_req_latency.labels.return_value = MagicMock()
        
        # Call the function directly
        response = self.inspect_file_func(self.valid_request)
        
        # Assertions
        self.assertIsInstance(response, InvokeMethodResponse)
        self.assertEqual(response.status_code, 200)
        
        # Verify the mocked file status was serialized correctly
        response_data = json.loads(response.data.decode())
        self.assertTrue(response_data["status"])
        self.assertEqual(response_data["path"], "/test/path/file.docx")
        self.assertEqual(response_data["error"], "")
        
        # Verify metrics calls
        mock_active_requests.labels.assert_called_with(method='inspect_file')
        mock_active_requests.labels.return_value.inc.assert_called_once()
        mock_active_requests.labels.return_value.dec.assert_called_once()
        
        mock_req_count.labels.assert_called_with(method='inspect_file', status='success')
        mock_req_count.labels.return_value.inc.assert_called_once()
        
        mock_req_latency.labels.assert_called_with(method='inspect_file')
        mock_req_latency.labels.return_value.observe.assert_called_once()
        
        # Verify file status call
        mock_get_file_status.assert_called_once()
        file_data = mock_get_file_status.call_args[0][0]
        self.assertIsInstance(file_data, FileData)
        self.assertEqual(file_data.file, self.valid_data["file"])
        self.assertEqual(file_data.application_id, self.valid_data["application_id"])

    @patch('app.pubsub.internal_functions.metrics_active_requests')
    @patch('app.pubsub.internal_functions.metrics_req_count')
    @patch('app.pubsub.internal_functions.metrics_req_latency')
    def test_inspect_file_validation_error(self, mock_req_latency, mock_req_count, 
                                         mock_active_requests):
        """Test validation error handling"""      
        mock_active_requests.labels.return_value = MagicMock()
        mock_req_count.labels.return_value = MagicMock()
        mock_req_latency.labels.return_value = MagicMock()
        
        # Call the function
        response = self.inspect_file_func(self.invalid_request)
        
        # Assertions
        self.assertIsInstance(response, InvokeMethodResponse)
        self.assertEqual(response.status_code, 400)
        
        # Verify metrics calls
        mock_active_requests.labels.assert_called_with(method='inspect_file')
        mock_active_requests.labels.return_value.inc.assert_called_once()
        mock_active_requests.labels.return_value.dec.assert_called_once()
        
        mock_req_count.labels.assert_called_with(method='inspect_file', status='validation_error')
        mock_req_count.labels.return_value.inc.assert_called_once()
        
        mock_req_latency.labels.assert_called_with(method='inspect_file')
        mock_req_latency.labels.return_value.observe.assert_called_once()

    @patch('app.pubsub.internal_functions.FileData')
    @patch('app.pubsub.internal_functions.instrumented_ext_get_file_status')
    @patch('app.pubsub.internal_functions.metrics_active_requests')
    @patch('app.pubsub.internal_functions.metrics_req_count')
    @patch('app.pubsub.internal_functions.metrics_req_latency')
    def test_inspect_file_general_exception(self, mock_req_latency, mock_req_count, 
                                          mock_active_requests, mock_get_file_status,
                                          mock_file_data):
        """Test general exception handling"""
        # Setup mocks
        mock_file_data.return_value = FileData(**self.valid_data)
        mock_get_file_status.side_effect = Exception("File processing error")
        
        mock_active_requests.labels.return_value = MagicMock()
        mock_req_count.labels.return_value = MagicMock()
        mock_req_latency.labels.return_value = MagicMock()
        
        # Call the function
        response = self.inspect_file_func(self.valid_request)
        
        # Assertions
        self.assertIsInstance(response, InvokeMethodResponse)
        self.assertEqual(response.status_code, 500)
        self.assertIn("File processing error", response.data.decode())
        
        # Verify metrics calls
        mock_active_requests.labels.assert_called_with(method='inspect_file')
        mock_active_requests.labels.return_value.inc.assert_called_once()
        mock_active_requests.labels.return_value.dec.assert_called_once()
        
        mock_req_count.labels.assert_called_with(method='inspect_file', status='error')
        mock_req_count.labels.return_value.inc.assert_called_once()
        
        mock_req_latency.labels.assert_called_with(method='inspect_file')
        mock_req_latency.labels.return_value.observe.assert_called_once()

    @patch('app.pubsub.internal_functions.json.loads')
    @patch('app.pubsub.internal_functions.metrics_active_requests')
    @patch('app.pubsub.internal_functions.metrics_req_count')
    @patch('app.pubsub.internal_functions.metrics_req_latency')
    def test_inspect_file_invalid_json(self, mock_req_latency, mock_req_count, 
                                     mock_active_requests, mock_json_loads):
        """Test invalid JSON handling"""
        # Setup mocks
        mock_json_loads.side_effect = json.JSONDecodeError("Invalid JSON", "", 0)
        
        mock_active_requests.labels.return_value = MagicMock()
        mock_req_count.labels.return_value = MagicMock()
        mock_req_latency.labels.return_value = MagicMock()
        
        # Create request with invalid JSON
        invalid_request = MagicMock(spec=InvokeMethodRequest)
        invalid_request.text.return_value = "{invalid json"
        invalid_request.metadata = {}
        
        # Call the function
        response = self.inspect_file_func(invalid_request)
        
        # Assertions
        self.assertIsInstance(response, InvokeMethodResponse)
        self.assertEqual(response.status_code, 500)
        
        # Verify metrics calls
        mock_active_requests.labels.assert_called_with(method='inspect_file')
        mock_active_requests.labels.return_value.inc.assert_called_once()
        mock_active_requests.labels.return_value.dec.assert_called_once()
        
        mock_req_count.labels.assert_called_with(method='inspect_file', status='error')
        mock_req_count.labels.return_value.inc.assert_called_once()
        
        mock_req_latency.labels.assert_called_with(method='inspect_file')
        mock_req_latency.labels.return_value.observe.assert_called_once()

    @patch('app.pubsub.internal_functions.time.perf_counter')
    @patch('app.pubsub.internal_functions.instrumented_ext_get_file_status')
    @patch('app.pubsub.internal_functions.metrics_active_requests')
    @patch('app.pubsub.internal_functions.metrics_req_count')
    @patch('app.pubsub.internal_functions.metrics_req_latency')
    def test_inspect_file_timing_measurement(self, mock_req_latency, mock_req_count,
                                           mock_active_requests, mock_get_file_status,
                                           mock_perf_counter):
        """Test that timing measurements are correct"""
        # Setup mocks
        mock_get_file_status.return_value = self.file_status_result
        mock_active_requests.labels.return_value = MagicMock()
        mock_req_count.labels.return_value = MagicMock()
        mock_req_latency.labels.return_value = MagicMock()
        
        # Mock the timer
        mock_perf_counter.side_effect = [100.0, 101.5]  # Start and end times (1.5s difference)
        
        # Call the function
        response = self.inspect_file_func(self.valid_request)
        
        # Assertions
        self.assertEqual(response.status_code, 200)
        
        # Verify timing measurement
        mock_req_latency.labels.assert_called_with(method='inspect_file')
        mock_req_latency.labels.return_value.observe.assert_called_once_with(1.5)


import unittest
from unittest.mock import patch, MagicMock
import json

from dapr.ext.grpc import InvokeMethodRequest, InvokeMethodResponse, App
from app.pubsub.models import ProtectFileData, UnprotectFileData
import app.pubsub.internal_functions

class TestProtectFile(unittest.TestCase):
    
    def setUp(self):
        # Setup test data
        self.valid_data = {
            "file": "/test/path/file.docx",
            "application_id": "test-app-id-123",
            "scc_token": "scc-token-123",
            "user": "test-user",
            "encrypted_file": "encrypted-content"
        }

        self.invalid_data = {
            "file": "/test/path/file.docx",
            "application_id": "test-app-id-123",
            "scc_token": "scc-token-123",
            # Missing required fields
        }
        
        # Mock request with valid data
        self.valid_request = MagicMock(spec=InvokeMethodRequest)
        self.valid_request.text.return_value = json.dumps(self.valid_data)
        self.valid_request.metadata = {"Content-Type": "application/json"}

        # Mock request with invalid data
        self.invalid_request = MagicMock(spec=InvokeMethodRequest)
        self.invalid_request.text.return_value = json.dumps(self.invalid_data)
        self.invalid_request.metadata = {"Content-Type": "application/json"}
        
        # Mock file status result
        self.protect_result = dict(
            status=True,
            path="/test/path/file.docx",
            error=""
        )
        
        # Get the function to test
        self.protect_file_func = app.pubsub.internal_functions.protect_file

    @patch('app.pubsub.internal_functions.instrumented_ext_protect_file')
    @patch('app.pubsub.internal_functions.metrics_active_requests')
    @patch('app.pubsub.internal_functions.metrics_req_count')
    @patch('app.pubsub.internal_functions.metrics_req_latency')
    def test_protect_file_success(self, mock_req_latency, mock_req_count, 
                                mock_active_requests, mock_ext_unprotect_file):
        """Test successful file protection by directly calling the function"""
        # Setup mocks
        mock_ext_unprotect_file.return_value = self.protect_result
        mock_active_requests.labels.return_value = MagicMock()
        mock_req_count.labels.return_value = MagicMock()
        mock_req_latency.labels.return_value = MagicMock()
        
        # Call the function directly
        response = self.protect_file_func(self.valid_request)
        
        # Assertions
        self.assertIsInstance(response, InvokeMethodResponse)
        self.assertEqual(response.status_code, 200)
        
        # Verify the response data
        response_data = json.loads(response.data.decode())
        self.assertTrue(response_data["status"])
        self.assertEqual(response_data["path"], "/test/path/file.docx")
        self.assertEqual(response_data["error"], "")
        
        # Verify metrics calls
        mock_active_requests.labels.assert_called_with(method='protect_file')
        mock_active_requests.labels.return_value.inc.assert_called_once()
        mock_active_requests.labels.return_value.dec.assert_called_once()
        
        mock_req_count.labels.assert_called_with(method='protect_file', status='success')
        mock_req_count.labels.return_value.inc.assert_called_once()
        
        mock_req_latency.labels.assert_called_with(method='protect_file')
        mock_req_latency.labels.return_value.observe.assert_called_once()
        
        # Verify external function call
        mock_ext_unprotect_file.assert_called_once()
        protect_data = mock_ext_unprotect_file.call_args[0][0]
        self.assertIsInstance(protect_data, ProtectFileData)
        self.assertEqual(protect_data.file, self.valid_data["file"])
        self.assertEqual(protect_data.application_id, self.valid_data["application_id"])
        self.assertEqual(protect_data.scc_token, self.valid_data["scc_token"])
        self.assertEqual(protect_data.user, self.valid_data["user"])
        self.assertEqual(protect_data.encrypted_file, self.valid_data["encrypted_file"])

    @patch('app.pubsub.internal_functions.metrics_active_requests')
    @patch('app.pubsub.internal_functions.metrics_req_count')
    @patch('app.pubsub.internal_functions.metrics_req_latency')
    def test_protect_file_validation_error(self, mock_req_latency, mock_req_count, 
                                         mock_active_requests):
        """Test validation error handling for protect_file"""
        mock_active_requests.labels.return_value = MagicMock()
        mock_req_count.labels.return_value = MagicMock()
        mock_req_latency.labels.return_value = MagicMock()
        
        # Call the function with invalid data
        response = self.protect_file_func(self.invalid_request)
        
        # Assertions
        self.assertIsInstance(response, InvokeMethodResponse)
        self.assertEqual(response.status_code, 400)
        
        # Verify metrics calls
        mock_active_requests.labels.assert_called_with(method='protect_file')
        mock_active_requests.labels.return_value.inc.assert_called_once()
        mock_active_requests.labels.return_value.dec.assert_called_once()
        
        mock_req_count.labels.assert_called_with(method='protect_file', status='validation_error')
        mock_req_count.labels.return_value.inc.assert_called_once()
        
        mock_req_latency.labels.assert_called_with(method='protect_file')
        mock_req_latency.labels.return_value.observe.assert_called_once()

    @patch('app.pubsub.internal_functions.ProtectFileData')
    @patch('app.pubsub.internal_functions.instrumented_ext_protect_file')
    @patch('app.pubsub.internal_functions.metrics_active_requests')
    @patch('app.pubsub.internal_functions.metrics_req_count')
    @patch('app.pubsub.internal_functions.metrics_req_latency')
    def test_protect_file_general_exception(self, mock_req_latency, mock_req_count, 
                                         mock_active_requests, mock_ext_unprotect_file,
                                         mock_protect_file_data):
        """Test general exception handling in protect_file"""
        # Setup mocks
        mock_protect_file_data.return_value = ProtectFileData(**self.valid_data)
        mock_ext_unprotect_file.side_effect = Exception("File processing error")
        
        mock_active_requests.labels.return_value = MagicMock()
        mock_req_count.labels.return_value = MagicMock()
        mock_req_latency.labels.return_value = MagicMock()
        
        # Call the function
        response = self.protect_file_func(self.valid_request)
        
        # Assertions
        self.assertIsInstance(response, InvokeMethodResponse)
        self.assertEqual(response.status_code, 500)
        self.assertIn("File processing error", response.data.decode())
        
        # Verify metrics calls
        mock_active_requests.labels.assert_called_with(method='protect_file')
        mock_active_requests.labels.return_value.inc.assert_called_once()
        mock_active_requests.labels.return_value.dec.assert_called_once()
        
        mock_req_count.labels.assert_called_with(method='protect_file', status='error')
        mock_req_count.labels.return_value.inc.assert_called_once()
        
        mock_req_latency.labels.assert_called_with(method='protect_file')
        mock_req_latency.labels.return_value.observe.assert_called_once()

    @patch('app.pubsub.internal_functions.json.loads')
    @patch('app.pubsub.internal_functions.metrics_active_requests')
    @patch('app.pubsub.internal_functions.metrics_req_count')
    @patch('app.pubsub.internal_functions.metrics_req_latency')
    def test_protect_file_invalid_json(self, mock_req_latency, mock_req_count, 
                                    mock_active_requests, mock_json_loads):
        """Test invalid JSON handling in protect_file"""
        # Setup mocks
        mock_json_loads.side_effect = json.JSONDecodeError("Invalid JSON", "", 0)
        
        mock_active_requests.labels.return_value = MagicMock()
        mock_req_count.labels.return_value = MagicMock()
        mock_req_latency.labels.return_value = MagicMock()
        
        # Create request with invalid JSON
        invalid_request = MagicMock(spec=InvokeMethodRequest)
        invalid_request.text.return_value = "{invalid json"
        invalid_request.metadata = {}
        
        # Call the function
        response = self.protect_file_func(invalid_request)
        
        # Assertions
        self.assertIsInstance(response, InvokeMethodResponse)
        self.assertEqual(response.status_code, 500)
        
        # Verify metrics calls
        mock_active_requests.labels.assert_called_with(method='protect_file')
        mock_active_requests.labels.return_value.inc.assert_called_once()
        mock_active_requests.labels.return_value.dec.assert_called_once()
        
        mock_req_count.labels.assert_called_with(method='protect_file', status='error')
        mock_req_count.labels.return_value.inc.assert_called_once()
        
        mock_req_latency.labels.assert_called_with(method='protect_file')
        mock_req_latency.labels.return_value.observe.assert_called_once()


class TestUnprotectFile(unittest.TestCase):
    
    def setUp(self):
        # Setup test data
        self.valid_data = {
            "file": "/test/path/file.docx",
            "application_id": "test-app-id-123",
            "scc_token": "scc-token-123"
        }

        self.invalid_data = {
            "file": "/test/path/file.docx",
            "application_id": "test-app-id-123"
            # Missing scc_token
        }
        
        # Mock request with valid data
        self.valid_request = MagicMock(spec=InvokeMethodRequest)
        self.valid_request.text.return_value = json.dumps(self.valid_data)
        self.valid_request.metadata = {"Content-Type": "application/json"}

        # Mock request with invalid data
        self.invalid_request = MagicMock(spec=InvokeMethodRequest)
        self.invalid_request.text.return_value = json.dumps(self.invalid_data)
        self.invalid_request.metadata = {"Content-Type": "application/json"}
        
        # Mock file status result
        self.unprotect_result = dict(
            status=True,
            path="/test/path/file.docx",
            error=""
        )
        
        # Get the function to test
        self.unprotect_file_func = app.pubsub.internal_functions.unprotect_file

    @patch('app.pubsub.internal_functions.instrumented_ext_unprotect_file')
    @patch('app.pubsub.internal_functions.metrics_active_requests')
    @patch('app.pubsub.internal_functions.metrics_req_count')
    @patch('app.pubsub.internal_functions.metrics_req_latency')
    def test_unprotect_file_success(self, mock_req_latency, mock_req_count, 
                                   mock_active_requests, mock_ext_protect_file):
        """Test successful file unprotection by directly calling the function"""
        # Setup mocks
        mock_ext_protect_file.return_value = self.unprotect_result
        mock_active_requests.labels.return_value = MagicMock()
        mock_req_count.labels.return_value = MagicMock()
        mock_req_latency.labels.return_value = MagicMock()
        
        # Call the function directly
        response = self.unprotect_file_func(self.valid_request)
        
        # Assertions
        self.assertIsInstance(response, InvokeMethodResponse)
        self.assertEqual(response.status_code, 200)
        
        # Verify the response data
        response_data = json.loads(response.data.decode())
        self.assertTrue(response_data["status"])
        self.assertEqual(response_data["path"], "/test/path/file.docx")
        self.assertEqual(response_data["error"], "")
        
        # Verify metrics calls
        mock_active_requests.labels.assert_called_with(method='unprotect_file')
        mock_active_requests.labels.return_value.inc.assert_called_once()
        mock_active_requests.labels.return_value.dec.assert_called_once()
        
        mock_req_count.labels.assert_called_with(method='unprotect_file', status='success')
        mock_req_count.labels.return_value.inc.assert_called_once()
        
        mock_req_latency.labels.assert_called_with(method='unprotect_file')
        mock_req_latency.labels.return_value.observe.assert_called_once()
        
        # Verify external function call
        mock_ext_protect_file.assert_called_once()
        unprotect_data = mock_ext_protect_file.call_args[0][0]
        self.assertIsInstance(unprotect_data, UnprotectFileData)
        self.assertEqual(unprotect_data.file, self.valid_data["file"])
        self.assertEqual(unprotect_data.application_id, self.valid_data["application_id"])
        self.assertEqual(unprotect_data.scc_token, self.valid_data["scc_token"])

    @patch('app.pubsub.internal_functions.metrics_active_requests')
    @patch('app.pubsub.internal_functions.metrics_req_count')
    @patch('app.pubsub.internal_functions.metrics_req_latency')
    def test_unprotect_file_validation_error(self, mock_req_latency, mock_req_count, 
                                           mock_active_requests):
        """Test validation error handling for unprotect_file"""
        mock_active_requests.labels.return_value = MagicMock()
        mock_req_count.labels.return_value = MagicMock()
        mock_req_latency.labels.return_value = MagicMock()
        
        # Call the function with invalid data
        response = self.unprotect_file_func(self.invalid_request)
        
        # Assertions
        self.assertIsInstance(response, InvokeMethodResponse)
        self.assertEqual(response.status_code, 400)
        
        # Verify metrics calls
        mock_active_requests.labels.assert_called_with(method='unprotect_file')
        mock_active_requests.labels.return_value.inc.assert_called_once()
        mock_active_requests.labels.return_value.dec.assert_called_once()
        
        mock_req_count.labels.assert_called_with(method='unprotect_file', status='validation_error')
        mock_req_count.labels.return_value.inc.assert_called_once()
        
        mock_req_latency.labels.assert_called_with(method='unprotect_file')
        mock_req_latency.labels.return_value.observe.assert_called_once()

    @patch('app.pubsub.internal_functions.UnprotectFileData')
    @patch('app.pubsub.internal_functions.instrumented_ext_unprotect_file')
    @patch('app.pubsub.internal_functions.metrics_active_requests')
    @patch('app.pubsub.internal_functions.metrics_req_count')
    @patch('app.pubsub.internal_functions.metrics_req_latency')
    def test_unprotect_file_general_exception(self, mock_req_latency, mock_req_count, 
                                            mock_active_requests, mock_ext_protect_file,
                                            mock_unprotect_file_data):
        """Test general exception handling in unprotect_file"""
        # Setup mocks
        mock_unprotect_file_data.return_value = UnprotectFileData(**self.valid_data)
        mock_ext_protect_file.side_effect = Exception("File processing error")
        
        mock_active_requests.labels.return_value = MagicMock()
        mock_req_count.labels.return_value = MagicMock()
        mock_req_latency.labels.return_value = MagicMock()
        
        # Call the function
        response = self.unprotect_file_func(self.valid_request)
        
        # Assertions
        self.assertIsInstance(response, InvokeMethodResponse)
        self.assertEqual(response.status_code, 500)
        self.assertIn("File processing error", response.data.decode())
        
        # Verify metrics calls
        mock_active_requests.labels.assert_called_with(method='unprotect_file')
        mock_active_requests.labels.return_value.inc.assert_called_once()
        mock_active_requests.labels.return_value.dec.assert_called_once()
        
        mock_req_count.labels.assert_called_with(method='unprotect_file', status='error')
        mock_req_count.labels.return_value.inc.assert_called_once()
        
        mock_req_latency.labels.assert_called_with(method='unprotect_file')
        mock_req_latency.labels.return_value.observe.assert_called_once()

    @patch('app.pubsub.internal_functions.json.loads')
    @patch('app.pubsub.internal_functions.metrics_active_requests')
    @patch('app.pubsub.internal_functions.metrics_req_count')
    @patch('app.pubsub.internal_functions.metrics_req_latency')
    def test_unprotect_file_invalid_json(self, mock_req_latency, mock_req_count, 
                                       mock_active_requests, mock_json_loads):
        """Test invalid JSON handling in unprotect_file"""
        # Setup mocks
        mock_json_loads.side_effect = json.JSONDecodeError("Invalid JSON", "", 0)
        
        mock_active_requests.labels.return_value = MagicMock()
        mock_req_count.labels.return_value = MagicMock()
        mock_req_latency.labels.return_value = MagicMock()
        
        # Create request with invalid JSON
        invalid_request = MagicMock(spec=InvokeMethodRequest)
        invalid_request.text.return_value = "{invalid json"
        invalid_request.metadata = {}
        
        # Call the function
        response = self.unprotect_file_func(invalid_request)
        
        # Assertions
        self.assertIsInstance(response, InvokeMethodResponse)
        self.assertEqual(response.status_code, 500)
        
        # Verify metrics calls
        mock_active_requests.labels.assert_called_with(method='unprotect_file')
        mock_active_requests.labels.return_value.inc.assert_called_once()
        mock_active_requests.labels.return_value.dec.assert_called_once()
        
        mock_req_count.labels.assert_called_with(method='unprotect_file', status='error')
        mock_req_count.labels.return_value.inc.assert_called_once()
        
        mock_req_latency.labels.assert_called_with(method='unprotect_file')
        mock_req_latency.labels.return_value.observe.assert_called_once()

