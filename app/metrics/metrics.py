import time
import functools
from prometheus_client import Counter, Histogram, Gauge
from app.pubsub.external_functions import ext_get_file_status, ext_protect_file, ext_unprotect_file


# Initialize Prometheus metrics
# Request count and exceptions
metrics_req_count = Counter(
    'msip_request_count', 
    'Count of requests received', 
    ['method', 'status']
)

# Request latency
metrics_req_latency = Histogram(
    'msip_request_latency_seconds', 
    'Time spent processing request',
    ['method'],
    buckets=(0.05, 0.1, 0.25, 0.5, 0.75, 1.0, 2.5, 5.0, 7.5, 10.0, 15.0, 30.0, 60.0)
)

# Call count for external C++ functions
metrics_ext_call_count = Counter(
    'msip_external_call_count', 
    'Count of calls to external C++ functions', 
    ['function', 'status']
)

# External function latency
ext_call_latency = Histogram(
    'msip_external_call_latency_seconds', 
    'Time spent in external C++ functions',
    ['function'],
    buckets=(0.01, 0.025, 0.05, 0.1, 0.25, 0.5, 0.75, 1.0, 2.5, 5.0)
)

# Active requests gauge
metrics_active_requests = Gauge(
    'msip_active_requests',
    'Number of requests currently being processed',
    ['method']
)


# Decorator to measure time spent in functions
def timing_decorator(metric, labels=None):
    if labels is None:
        labels = {}
    
    def decorator(func):
        @functools.wraps(func)
        def wrapped_func(*args, **kwargs):
            if isinstance(metric, Histogram):
                with metric.labels(**labels).time():
                    return func(*args, **kwargs)
            else:
                start = time.perf_counter()
                try:
                    return func(*args, **kwargs)
                finally:
                    metric.labels(**labels).observe(time.perf_counter() - start)
        return wrapped_func
    return decorator


@timing_decorator(ext_call_latency, {'function': 'get_file_status'})
def instrumented_ext_get_file_status(data):
    try:
        result = ext_get_file_status(data)
        metrics_ext_call_count.labels(function='get_file_status', status='success').inc()
        return result
    except Exception as e:
        metrics_ext_call_count.labels(function='get_file_status', status='error').inc()
        raise

@timing_decorator(ext_call_latency, {'function': 'protect_file'})
def instrumented_ext_protect_file(data):
    try:
        result = ext_protect_file(data)
        metrics_ext_call_count.labels(function='protect_file', status='success').inc()
        return result
    except Exception as e:
        metrics_ext_call_count.labels(function='protect_file', status='error').inc()
        raise

@timing_decorator(ext_call_latency, {'function': 'unprotect_file'})
def instrumented_ext_unprotect_file(data):
    try:
        result = ext_unprotect_file(data)
        metrics_ext_call_count.labels(function='unprotect_file', status='success').inc()
        return result
    except Exception as e:
        metrics_ext_call_count.labels(function='unprotect_file', status='error').inc()
        raise
