import ctypes
import logging
import json
from app.core.settings import settings
from app.pubsub.models import FileData, ProtectFileData, UnprotectFileData

logger = logging.getLogger(__name__)


# Load the shared library
msip_lib = ctypes.CDLL(settings.MSIP_LD_PATH)

# Get the function and specify argument types and return type
get_file_status = msip_lib.getFileStatus
get_file_status.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p]
get_file_status.restype = ctypes.c_int

# Get the function and specify argument types and return type
unprotect_file = msip_lib.unprotectFile
unprotect_file.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p]
unprotect_file.restype = ctypes.c_int

protect_file = msip_lib.protectFile
protect_file.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p]
protect_file.restype = ctypes.c_int


def ext_get_file_status(data: FileData) -> dict:

    # Create buffer for result
    result_buffer = ctypes.create_string_buffer(8192)

    # Call the function
    get_file_status(data.file.encode(), data.application_id.encode(), result_buffer)
    # Parse the JSON result
    try:
        # Decode the bytes to string, then parse as JSON
        json_str = result_buffer.value.decode('utf-8')
        return json.loads(json_str)
    except json.JSONDecodeError as e:
        logger.exception("Failed to parse response: %s", e)
        return {
            "path": data.file,
            "status": False,
            "error": str(e),
            "raw": result_buffer.value
        }

def ext_unprotect_file(data: UnprotectFileData) -> dict:
    # Create buffer for result
    result_buffer = ctypes.create_string_buffer(8192)

    # Call the function
    ret_val = unprotect_file(
        data.scc_token.encode(),
        data.file.encode(),
        data.application_id.encode(),
        result_buffer
    )

    # Print return code
    logger.info(f"ext_unprotect_file Return code: {ret_val}, result: {result_buffer.value.decode('utf-8')}")

    # Parse the JSON result
    try:
        # Decode the bytes to string, then parse as JSON
        json_str = result_buffer.value.decode('utf-8')
        return json.loads(json_str)
    except json.JSONDecodeError as e:
        logger.exception("Failed to parse response: %s", e)
        return {
            "path": data.file,
            "status": False,
            "error": str(e),
            "raw": result_buffer.value
        }

def ext_protect_file(data: ProtectFileData) -> dict:
    # Create buffer for result
    result_buffer = ctypes.create_string_buffer(8192)

    # Call the function
    protect_file(
        data.scc_token.encode(),
        data.file.encode(),
        data.encrypted_file.encode(),
        data.user.encode(),
        data.application_id.encode(),
        result_buffer
    )
    # Parse the JSON result
    try:
        # Decode the bytes to string, then parse as JSON
        json_str = result_buffer.value.decode('utf-8')
        return json.loads(json_str)       
    except json.JSONDecodeError as e:
        logger.exception("Failed to parse response: %s", e)
        return {
            "path": data.file,
            "status": False,
            "error": str(e),
            "raw": result_buffer.value
        }
