import json
import time
import logging
from dapr.ext.grpc import InvokeMethodRequest, InvokeMethodResponse
from pydantic import ValidationError
from app.pubsub.models import FileData, ProtectFileData, UnprotectFileData
from app.metrics.metrics import (
        instrumented_ext_get_file_status, instrumented_ext_protect_file, instrumented_ext_unprotect_file,
        metrics_active_requests, metrics_req_count, metrics_req_latency
)

logger = logging.getLogger(__name__)


def inspect_file(request: InvokeMethodRequest) -> InvokeMethodResponse:
    method_name = 'inspect_file'
    metrics_active_requests.labels(method=method_name).inc()
    start_time = time.perf_counter()
    
    logger.info('--------------Received inspect_file invocation -----------------------------------------------')
    
    try:
        data = json.loads(request.text())
        data = FileData(**data)
        result = instrumented_ext_get_file_status(data)
        response = InvokeMethodResponse(json.dumps(result).encode(), "application/json", status_code=200)
        metrics_req_count.labels(method=method_name, status='success').inc()
        return response
    except ValidationError as e:
        logger.info(e)
        logger.exception(f"Validation error in {method_name}: {e}")
        metrics_req_count.labels(method=method_name, status='validation_error').inc()
        return InvokeMethodResponse(str(e), "application/json", status_code=400)
    except Exception as e:
        logger.exception(f"Error in {method_name}: {type(e)}")
        metrics_req_count.labels(method=method_name, status='error').inc()
        return InvokeMethodResponse(str(e), "application/json", status_code=500)
    finally:
        metrics_req_latency.labels(method=method_name).observe(time.perf_counter() - start_time)
        metrics_active_requests.labels(method=method_name).dec()


def unprotect_file(request: InvokeMethodRequest) -> InvokeMethodResponse:
    method_name = 'unprotect_file'
    metrics_active_requests.labels(method=method_name).inc()
    start_time = time.perf_counter()
    
    logger.info('--------------Received unprotect_file invocation -----------------------------------------------')   
    try:
        data = json.loads(request.text())
        data = UnprotectFileData(**data)
        result = instrumented_ext_unprotect_file(data)
        response = InvokeMethodResponse(json.dumps(result).encode(), "application/json", status_code=200)
        metrics_req_count.labels(method=method_name, status='success').inc()
        return response
    except ValidationError as e:
        logger.info(e)
        metrics_req_count.labels(method=method_name, status='validation_error').inc()
        return InvokeMethodResponse(str(e), "application/json", status_code=400)
    except Exception as e:
        logger.exception(f"Error in {method_name}")
        metrics_req_count.labels(method=method_name, status='error').inc()
        return InvokeMethodResponse(str(e), "application/json", status_code=500)
    finally:
        metrics_req_latency.labels(method=method_name).observe(time.perf_counter() - start_time)
        metrics_active_requests.labels(method=method_name).dec()

def protect_file(request: InvokeMethodRequest) -> InvokeMethodResponse:
    method_name = 'protect_file'
    metrics_active_requests.labels(method=method_name).inc()
    start_time = time.perf_counter()
    
    logger.info('--------------Received protect_file invocation -----------------------------------------------')
    
    try:
        data = json.loads(request.text())
        data = ProtectFileData(**data)
        result = instrumented_ext_protect_file(data)
        response = InvokeMethodResponse(json.dumps(result).encode(), "application/json", status_code=200)
        metrics_req_count.labels(method=method_name, status='success').inc()
        return response
    except ValidationError as e:
        logger.info(e)
        metrics_req_count.labels(method=method_name, status='validation_error').inc()
        return InvokeMethodResponse(str(e), "application/json", status_code=400)
    except Exception as e:
        logger.exception(f"Error in {method_name}")
        metrics_req_count.labels(method=method_name, status='error').inc()
        return InvokeMethodResponse(str(e), "application/json", status_code=500)
    finally:
        metrics_req_latency.labels(method=method_name).observe(time.perf_counter() - start_time)
        metrics_active_requests.labels(method=method_name).dec()
