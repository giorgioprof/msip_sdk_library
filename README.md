# AIP File Protection Service

A gRPC-based Dapr service for inspecting, protecting, and unprotecting files using Microsoft Information Protection.

## Overview

AIP File Protection Service is a microservice that provides a simple, reliable interface to the Microsoft Information Protection (MIP) SDK. It leverages Dapr for service discovery, invocation, and metrics collection, and exposes gRPC endpoints for file protection operations.

The service is designed to be deployable in Kubernetes or any containerized environment that supports Dapr.

## Features

- **File Inspection**: Examine files to determine protection status
- **File Protection**: Apply protection to files with specified security controls
- **File Unprotection**: Remove protection from protected files
- **Metrics**: Prometheus metrics for monitoring service health and performance
- **Containerized**: Fully containerized deployment with minimal dependencies
- **Dapr Integration**: Built-in Dapr support for easy service invocation

## Available gRPC Methods

The service exposes three main methods via Dapr gRPC:

1. **inspect_file** - Examines a file to determine its protection status
2. **protect_file** - Applies protection to a file
3. **unprotect_file** - Removes protection from a protected file

## API Reference

### inspect_file

Retrieves the protection status of a file.

**Request:**
```json
{
 "file": "/path/to/Protected file",
 "application_id": "your-azure-application-id"
}
```

**Response**:
```json
{
  "status": true/false,
  "path": "/path/to/Protected file",
  "error": "",
  "protected": true/false,
  "labeled": true/false,
  "protected_objects": true/false
}
```
If status is true, then the **protected** property defines whether the file is DRM protected. If there was an error during processing, the error field will contain details.

### protect_file

Applies protection to a file.
**Request**:
```json
{
  "file": "/path/to/Unrotected file",
  "application_id": "your-application-id",
  "scc_token": "security-control-token",
  "user": "user@example.com",
  "encrypted_file": "/path/to/Protected file"  // Used to extract the original encrypting polichy and apply it to the new file
}
```

**Response**:
```json
{
  "status": true,
  "path": "/path/to/Protected file",
  "error": ""
}
```
A successful protection operation returns status: true. Any errors will be detailed in the error field.

### unprotect_file

Removes protection from a file.

**Request**:

```json
{
  "file": "/path/to/Protected file",
  "application_id": "your-application-id",
  "scc_token": "security-control-token"
}
```

**Response**:
```json
{
  "status": true,
  "path": "/path/to/Unprotected file",
  "error": ""
}
```
A successful unprotection operation returns status: true. Any errors will be detailed in the error field.

## How to Use with Dapr
### Python Client Example

```python
import grpc
import json
from dapr.clients import DaprClient
from dapr.clients.grpc._request import InvokeMethodRequest

# Initialize Dapr client
with DaprClient() as client:
    # Create request data
    request_data = {
        "file": "/path/to/document.docx",
        "application_id": "app-id-12345"
    }
    
    # Create invoke request
    request = InvokeMethodRequest(
        app_id="aip-file-service",
        method_name="inspect_file",
        data=json.dumps(request_data).encode('utf-8'),
        content_type="application/json"
    )
    
    # Invoke method
    response = client.invoke_method(request)
    
    # Process response
    result = json.loads(response.text())    
    if result['status']:
        print(f'File is protected: {result["protected"]}'))
    else:
        print(f'Failed to inspect file with error: {result["error"]}')
```


## Deployment
### Docker Image
The service is packaged as a Docker image and can be deployed in any container orchestration platform. The image includes the necessary libraries and dependencies for the MIP SDK.
 #### Kubernetes Deployment
```yaml
yamlapiVersion: apps/v1
kind: Deployment
metadata:
  name: msip-file-handler-microservice
  labels:
    app: msip-file-handler-microservice
spec:
  replicas: 3
  selector:
    matchLabels:
      app: msip-file-handler-microservice
  template:
    metadata:
      labels:
        app: msip-file-handler-microservice
      annotations:
        dapr.io/enabled: "true"
        dapr.io/app-id: "msip-file-handler-microservice"
        dapr.io/app-port: "50051"
        dapr.io/app-protocol: "grpc"
        prometheus.io/scrape: "true"
        prometheus.io/path: "/metrics"
        prometheus.io/port: "8000"
    spec:
      containers:
      - name: msip-file-handler-microservice
        image: your-registry/msip-file-handler-microservice:latest
        ports:
        - containerPort: 50051
          name: grpc
        - containerPort: 8000
          name: metrics
        env:
        - name: GRPC_PORT
          value: "50051"
        - name: PROMETHEUS_PORT
          value: "8000"
        resources:
          limits:
            cpu: "1"
            memory: "1Gi"
          requests:
            cpu: "0.5"
            memory: "512Mi"
```

## Environment Variables
- GRPC_PORT: Port for the gRPC server (default: 50051)
- PROMETHEUS_PORT: Port for Prometheus metrics (default: 8000)


## Monitoring and Observability
### Metrics

The service exposes Prometheus metrics on the configured PROMETHEUS_PORT including:

- **Request Counts**: Total number of requests by method and status
- **Request Latency**: Histogram of request processing times
- **Active Requests**: Gauge of currently processing requests
- **External Function Calls**: Counts and latencies of calls to the MIP SDK

## Scaling
The service is designed to be horizontally scalable. The main considerations for scaling are:

- CPU Usage: The MIP SDK operations are CPU-intensive
- Memory Usage: Large files may require significant memory
- Concurrency: The service handles multiple requests concurrently, limited by the thread pool size (default 10)

### Recommended starting point for resource allocation:

- CPU: 0.5-1 CPU cores per instance
- Memory: 512MB-1GB per instance
- Instances: 3-5 instances for moderate load

