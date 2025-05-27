import logging
from logging.config import dictConfig
import json
import sentry_sdk
from pathlib import Path
from pydantic_settings import BaseSettings


class EnvSettings(BaseSettings):

    NAME: str = 'msip_file_handler_microservice'

    MSIP_LD_PATH: Path = Path('/app/lib/aip_file.so')

    BASE_DIR: Path = Path(__file__).resolve().parent.parent

    ENVIRONMENT: str = 'dev'

    PROMETHEUS_PORT: int = 8000
    GRPC_PORT: int = 50051

    
    # Sentry
    SENTRY_DSN: str | None = None
    SENTRY_RELEASE: str = "v1.0"

settings = EnvSettings()

# setup loggers
with open(f'{settings.BASE_DIR}/etc/logging.json', 'r', encoding='utf-8') as logging_conf_dict:
    dictConfig(json.loads(logging_conf_dict.read()))

if settings.SENTRY_DSN:  # pragma: no cover
    sentry_sdk.init(
        dsn=settings.SENTRY_DSN,
        environment=settings.ENVIRONMENT,
        release=f"{settings.NAME} {settings.SENTRY_RELEASE}",
        # Set traces_sample_rate to 1.0 to capture 100%
        # of transactions for performance monitoring.
        traces_sample_rate=0.2,
        profiles_sample_rate=0.2,
        max_value_length=4000,
    )
    
