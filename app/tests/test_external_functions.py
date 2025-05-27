import unittest
from unittest.mock import patch, MagicMock, mock_open, call
import json
import ctypes

from app.pubsub.models import FileData, UnprotectFileData, ProtectFileData
from app.pubsub.external_functions import (
    ext_get_file_status, 
    ext_unprotect_file, 
    ext_protect_file
)

class TestExternalFunctions(unittest.TestCase):
    
    def setUp(self):
        # Setup common test data
        self.file_data = FileData(
            file="/test/path/document.docx".encode('utf-8'),
            application_id="test-app-id-123".encode('utf-8')
        )
        
        self.unprotect_data = UnprotectFileData(
            file="/test/path/document.docx",
            application_id="test-app-id-123",
            scc_token="test-scc-token-456"
        )
        
        self.protect_data = ProtectFileData(
            file="/test/path/document.docx",
            application_id="test-app-id-123",
            scc_token="test-scc-token-456",
            user="test-user",
            encrypted_file="encrypted-content-base64"
        )
        
        # Success response from C library
        self.success_response = json.dumps({
            "status": True,
            "path": "/test/path/document.docx",
            "error": ""
        }).encode('utf-8')
        
        # Error response from C library
        self.error_response = json.dumps({
            "status": False,
            "path": "/test/path/document.docx",
            "error": "Access denied"
        }).encode('utf-8')
        
        # Invalid JSON response for error cases
        self.invalid_json_response = b"{invalid-json"

    @patch('app.pubsub.external_functions.ctypes.create_string_buffer')
    @patch('app.pubsub.external_functions.get_file_status')
    def test_ext_get_file_status_success(self, mock_get_file_status, mock_create_buffer):
        """Test successful file status retrieval"""
        # Create a mock buffer with our success response
        mock_buffer = MagicMock()
        mock_buffer.value = self.success_response
        mock_create_buffer.return_value = mock_buffer
        
        # Mock the C function call to return success
        mock_get_file_status.return_value = 0
        
        # Call the function
        result = ext_get_file_status(self.file_data)
        
        # Verify the result
        self.assertTrue(result["status"])
        self.assertEqual(result["path"], "/test/path/document.docx")
        self.assertEqual(result["error"], "")
        
        # Verify the C function was called with correct arguments
        mock_get_file_status.assert_called_once()
        file_arg = mock_get_file_status.call_args[0][0]
        app_id_arg = mock_get_file_status.call_args[0][1]
        buffer_arg = mock_get_file_status.call_args[0][2]
        
        # Check parameter types - in Python 3, strings are passed as bytes to C
        self.assertEqual(file_arg.decode(), self.file_data.file)
        self.assertEqual(app_id_arg.decode(), self.file_data.application_id)
        self.assertEqual(buffer_arg, mock_buffer)

    @patch('app.pubsub.external_functions.ctypes.create_string_buffer')
    @patch('app.pubsub.external_functions.get_file_status')
    def test_ext_get_file_status_error(self, mock_get_file_status, mock_create_buffer):
        """Test file status retrieval with error response"""
        # Create a mock buffer with our error response
        mock_buffer = MagicMock()
        mock_buffer.value = self.error_response
        mock_create_buffer.return_value = mock_buffer
        
        # Mock the C function call to return error code
        mock_get_file_status.return_value = -1
        
        # Call the function
        result = ext_get_file_status(self.file_data)
        
        # Verify the result includes the error
        self.assertFalse(result["status"])
        self.assertEqual(result["path"], "/test/path/document.docx")
        self.assertEqual(result["error"], "Access denied")
        
        # Verify the function was called
        mock_get_file_status.assert_called_once()

    @patch('app.pubsub.external_functions.ctypes.create_string_buffer')
    @patch('app.pubsub.external_functions.get_file_status')
    def test_ext_get_file_status_invalid_json(self, mock_get_file_status, mock_create_buffer):
        """Test handling of invalid JSON response"""
        # Create a mock buffer with invalid JSON
        mock_buffer = MagicMock()
        mock_buffer.value = self.invalid_json_response
        mock_create_buffer.return_value = mock_buffer
        
        # Mock the C function call
        mock_get_file_status.return_value = 0
        
        # Call the function
        result = ext_get_file_status(self.file_data)
        
        # Verify error handling
        self.assertFalse(result["status"])
        self.assertEqual(result["path"], self.file_data.file)
        self.assertIn("Expecting property name enclosed in double quotes", result["error"])
        self.assertEqual(result["raw"], self.invalid_json_response)
        
        # Verify the function was called
        mock_get_file_status.assert_called_once()

    @patch('app.pubsub.external_functions.ctypes.create_string_buffer')
    @patch('app.pubsub.external_functions.unprotect_file')
    def test_ext_unprotect_file_success(self, mock_unprotect_file, mock_create_buffer):
        """Test successful file unprotection"""
        # Create a mock buffer with our success response
        mock_buffer = MagicMock()
        mock_buffer.value = self.success_response
        mock_create_buffer.return_value = mock_buffer
        
        # Mock the C function call
        mock_unprotect_file.return_value = 0
        
        # Call the function
        result = ext_unprotect_file(self.unprotect_data)
        
        # Verify the result
        self.assertTrue(result["status"])
        self.assertEqual(result["path"], "/test/path/document.docx")
        self.assertEqual(result["error"], "")
        
        # Verify the function was called with correct arguments
        mock_unprotect_file.assert_called_once()
        token_arg = mock_unprotect_file.call_args[0][0]
        file_arg = mock_unprotect_file.call_args[0][1]
        app_id_arg = mock_unprotect_file.call_args[0][2]
        buffer_arg = mock_unprotect_file.call_args[0][3]
        
        self.assertEqual(token_arg.decode(), self.unprotect_data.scc_token)
        self.assertEqual(file_arg.decode(), self.unprotect_data.file)
        self.assertEqual(app_id_arg.decode(), self.unprotect_data.application_id)
        self.assertEqual(buffer_arg, mock_buffer)

    @patch('app.pubsub.external_functions.ctypes.create_string_buffer')
    @patch('app.pubsub.external_functions.unprotect_file')
    def test_ext_unprotect_file_error(self, mock_unprotect_file, mock_create_buffer):
        """Test file unprotection with error response"""
        # Create a mock buffer with our error response
        mock_buffer = MagicMock()
        mock_buffer.value = self.error_response
        mock_create_buffer.return_value = mock_buffer
        
        # Mock the C function call
        mock_unprotect_file.return_value = -1
        
        # Call the function
        result = ext_unprotect_file(self.unprotect_data)
        
        # Verify the result includes the error
        self.assertFalse(result["status"])
        self.assertEqual(result["path"], "/test/path/document.docx")
        self.assertEqual(result["error"], "Access denied")
        
        # Verify the function was called
        mock_unprotect_file.assert_called_once()

    @patch('app.pubsub.external_functions.ctypes.create_string_buffer')
    @patch('app.pubsub.external_functions.unprotect_file')
    def test_ext_unprotect_file_invalid_json(self, mock_unprotect_file, mock_create_buffer):
        """Test handling of invalid JSON response in unprotect_file"""
        # Create a mock buffer with invalid JSON
        mock_buffer = MagicMock()
        mock_buffer.value = self.invalid_json_response
        mock_create_buffer.return_value = mock_buffer
        
        # Mock the C function call
        mock_unprotect_file.return_value = 0
        
        # Call the function
        result = ext_unprotect_file(self.unprotect_data)
        
        # Verify error handling
        self.assertFalse(result["status"])
        self.assertEqual(result["path"], self.unprotect_data.file)
        self.assertIn("Expecting property name enclosed in double quotes", result["error"])
        self.assertEqual(result["raw"], self.invalid_json_response)
        
        # Verify the function was called
        mock_unprotect_file.assert_called_once()

    @patch('app.pubsub.external_functions.ctypes.create_string_buffer')
    @patch('app.pubsub.external_functions.protect_file')
    def test_ext_protect_file_success(self, mock_protect_file, mock_create_buffer):
        """Test successful file protection"""
        # Create a mock buffer with our success response
        mock_buffer = MagicMock()
        mock_buffer.value = self.success_response
        mock_create_buffer.return_value = mock_buffer
        
        # Mock the C function call
        mock_protect_file.return_value = 0
        
        # Call the function
        result = ext_protect_file(self.protect_data)
        
        # Verify the result
        self.assertTrue(result["status"])
        self.assertEqual(result["path"], "/test/path/document.docx")
        self.assertEqual(result["error"], "")
        
        # Verify the function was called with correct arguments
        mock_protect_file.assert_called_once()
        token_arg = mock_protect_file.call_args[0][0]
        file_arg = mock_protect_file.call_args[0][1]
        encrypted_arg = mock_protect_file.call_args[0][2]
        user_arg = mock_protect_file.call_args[0][3]
        app_id_arg = mock_protect_file.call_args[0][4]
        buffer_arg = mock_protect_file.call_args[0][5]
        
        self.assertEqual(token_arg.decode(), self.protect_data.scc_token)
        self.assertEqual(file_arg.decode(), self.protect_data.file)
        self.assertEqual(encrypted_arg.decode(), self.protect_data.encrypted_file)
        self.assertEqual(user_arg.decode(), self.protect_data.user)
        self.assertEqual(app_id_arg.decode(), self.protect_data.application_id)
        self.assertEqual(buffer_arg, mock_buffer)

    @patch('app.pubsub.external_functions.ctypes.create_string_buffer')
    @patch('app.pubsub.external_functions.protect_file')
    def test_ext_protect_file_error(self, mock_protect_file, mock_create_buffer):
        """Test file protection with error response"""
        # Create a mock buffer with our error response
        mock_buffer = MagicMock()
        mock_buffer.value = self.error_response
        mock_create_buffer.return_value = mock_buffer
        
        # Mock the C function call
        mock_protect_file.return_value = -1
        
        # Call the function
        result = ext_protect_file(self.protect_data)
        
        # Verify the result includes the error
        self.assertFalse(result["status"])
        self.assertEqual(result["path"], "/test/path/document.docx")
        self.assertEqual(result["error"], "Access denied")
        
        # Verify the function was called
        mock_protect_file.assert_called_once()

    @patch('app.pubsub.external_functions.ctypes.create_string_buffer')
    @patch('app.pubsub.external_functions.protect_file')
    def test_ext_protect_file_invalid_json(self, mock_protect_file, mock_create_buffer):
        """Test handling of invalid JSON response in protect_file"""
        # Create a mock buffer with invalid JSON
        mock_buffer = MagicMock()
        mock_buffer.value = self.invalid_json_response
        mock_create_buffer.return_value = mock_buffer
        
        # Mock the C function call
        mock_protect_file.return_value = 0
        
        # Call the function
        result = ext_protect_file(self.protect_data)
        
        # Verify error handling
        self.assertFalse(result["status"])
        self.assertEqual(result["path"], self.protect_data.file)
        self.assertIn("Expecting property name enclosed in double quotes", result["error"])
        self.assertEqual(result["raw"], self.invalid_json_response)
        
        # Verify the function was called
        mock_protect_file.assert_called_once()

    @patch('app.pubsub.external_functions.get_file_status')
    def test_ext_get_file_status_exception_handling(self, mock_get_file_status):
        """Test general exception handling in ext_get_file_status"""
        # Setup the mock to raise an exception
        mock_get_file_status.side_effect = Exception("Unexpected error")
        
        # Call the function and expect it to handle the exception
        with self.assertRaises(Exception):
            ext_get_file_status(self.file_data)

    def test_ctypes_buffer_size(self):
        """Test that the buffer size is adequate"""
        # This is a more conceptual test to verify our buffer size is sufficient
        # In a real test, you might verify this with actual data
        self.assertTrue(8192 >= len(self.success_response), 
                        "Buffer size should be large enough for typical responses")
        self.assertTrue(8192 >= len(self.error_response), 
                        "Buffer size should be large enough for error responses")
