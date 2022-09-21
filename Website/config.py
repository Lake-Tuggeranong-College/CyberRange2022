import os
basedir = os.path.abspath(os.path.dirname(__file__))


class Config(object):
    SECRET_KEY = os.environ.get('SECRET_KEY') or '1234'
    # Database configuration
    # SQLALCHEMY_DATABASE_URI = os.environ.get('DATABASE_URL') or 'sqlite:///' + os.path.join(basedir, 'app.db')
    SQLALCHEMY_DATABASE_URI = "mysql+pymysql://CTF:admin@10.76.43.33/CTF"
    SQLALCHEMY_TRACK_MODIFICATIONS = False