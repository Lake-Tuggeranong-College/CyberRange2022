from app import db
from werkzeug.security import generate_password_hash, check_password_hash
from flask_login import UserMixin
from app import login

class User(UserMixin, db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(64), index=True, unique=True)
    username = db.Column(db.String(64), index=True, unique=True)
    email = db.Column(db.String(120), index=True, unique=True)
    password_hash = db.Column(db.String(128))
    is_administrator = db.Column(db.Boolean)
    current_score = db.Column(db.Integer)
    active_player = db.Column(db.Boolean)

    def __init__(self, name, username, email):
        self.username = username
        self.name = name
        self.email = email
        self.active_player = 1
        self.current_score = 0
        self.is_administrator = 0

    def reset_score(self):
        self.current_score=0
    def __repr__(self):
        return '<User {}>'.format(self.username)

    def update_details(self, username, name, email):
        self.username = username
        self.email = email
        self.name = name
        print(self)

    def set_password(self, password):
        self.password_hash = generate_password_hash(password)

    def check_password(self, password):
        return check_password_hash(self.password_hash, password)

    def is_admin(self):
        if self.is_administrator == bool('true'):
            print("{} is an Administrator".format(self.username))
            return True
        else:
            print("{} is a Regular User".format(self.username))
            return False


@login.user_loader
def load_user(id):
    return User.query.get(int(id))


class CTFSubSystems(db.Model):
    subsystemid = db.Column(db.Integer, primary_key=True)
    title = db.Column(db.String(64), index=True, unique=True)
    description = db.Column(db.String(256), index=True, unique=True)
    Owner = db.Column(db.String(64), index=True, unique=False)
    score = db.Column(db.Integer, index=True, unique=False)
    status = db.Column(db.Boolean, index=True, unique=False)
    Code = db.Column(db.String(11), index=True, unique=True)
    ip_address = db.Column(db.String())
    challenge_text = db.Column(db.String())
    active=db.Column(db.Boolean)
    hints = db.Column(db.String(1024))


    def set_passcode(self, code):
        self.Code = generate_password_hash(code)

    def check_passcode(self, code):
        return check_password_hash(self.Code, code)

    def claim(self):
        self.status = True
        print("claimed")

    def reset(self):
        self.status = True
        print("reset")

class solved_modules (db.Model):
    id = db.Column(db.Integer, primary_key=True)
    userid = db.Column(db.Integer)
    moduleid = db.Column(db.Integer)