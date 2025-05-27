import logging
import threading
from app.core.settings import settings
from dapr.ext.grpc import App, InvokeMethodRequest, InvokeMethodResponse
from prometheus_client import start_http_server
from app.pubsub.internal_functions import inspect_file, protect_file, unprotect_file

logger = logging.getLogger(__name__)


dapr_grpc = App()

@dapr_grpc.method(name='inspect_file')
def dapr_inspect_file(request: InvokeMethodRequest) -> InvokeMethodResponse:
    return inspect_file(request)

@dapr_grpc.method(name='protect_file')
def dapr_protect_file(request: InvokeMethodRequest) -> InvokeMethodResponse:
    return protect_file(request)

@dapr_grpc.method(name='unprotect_file')
def dapr_unprotect_file(request: InvokeMethodRequest) -> InvokeMethodResponse:
    return unprotect_file(request)


def start_prometheus_server(port: int = 8000):
    """Start Prometheus HTTP server in a separate thread"""
    # This is the key part - starting the server in a new thread
    thread = threading.Thread(
        target=lambda: start_http_server(port),
        daemon=True  # This ensures the thread exits when the main program exits
    )
    thread.start()
    logger.info(f"Prometheus metrics server started on port {port}")
    return thread


if __name__ == "__main__":
    # Start Prometheus server
    start_prometheus_server(settings.PROMETHEUS_PORT)
    
    logger.info('Starting pubsub consumer with Prometheus metrics enabled')
    logger.info(f'Metrics available at http://localhost:{settings.PROMETHEUS_PORT}/metrics')
    
    # Start the Dapr gRPC server
    dapr_grpc.run(settings.GRPC_PORT)