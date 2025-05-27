import os


# Set these here to avoid EnvSettings breaking
# when imported in tests further below
os.environ["MYSQL_USER"] = ""
os.environ["MYSQL_PASSWORD"] = ""
os.environ["MYSQL_DB"] = ""
os.environ["MYSQL_HOST"] = ""
