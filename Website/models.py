from app import db
from werkzeug.security import generate_password_hash, check_password_hash
from flask_login import UserMixin
from app import login
from datetime import datetime


class User(UserMixin, db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(64), index=True, unique=True)
    username = db.Column(db.String(64), index=True, unique=True)
    email = db.Column(db.String(120), index=True, unique=True)
    password_hash = db.Column(db.String(128))
    is_administrator = db.Column(db.Boolean)
    current_score = db.Column(db.Integer)
    active_player = db.Column(db.Boolean)

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


class Order(db.Model):
    claimID = db.Column(db.Integer, primary_key=True)
    subsystemID = db.Column(db.Integer, db.ForeignKey('product.productID'))
    userID = db.Column(db.Integer, db.ForeignKey('user.id'))
    claimdate = db.Column(db.DateTime, index=False, unique=False)

    def __init__(self, userid, subsystemid, claim_date=datetime.today()):
        self.userID = userid
        self.subsystemID = subsystemid
        self.claimdate = claim_date

    def __repr__(self):
        return '<Claim {}: User {} Claimed Subsystems {}, date: {}>'.format(self.claimID, self.userID, self.subsystemID, self.orderdate)
