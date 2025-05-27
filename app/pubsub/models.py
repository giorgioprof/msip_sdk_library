from pydantic import BaseModel


class FileData(BaseModel):
    file: str    
    application_id: str


class UnprotectFileData(FileData):
    scc_token: str


class ProtectFileData(UnprotectFileData):
    user: str
    encrypted_file: str
