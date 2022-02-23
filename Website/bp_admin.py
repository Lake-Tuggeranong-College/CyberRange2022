# All the code/routes for the administration will be stored here.
from flask import Flask, url_for, render_template, redirect, flash, request, Markup, Blueprint
from flask_admin import Admin
from flask_admin.contrib.sqla import ModelView
from models import User, CTFSubSystems

from app import db, app

admin_blueprint = Blueprint('admin_blueprint', __name__)

# Flask and Flask-SQLAlchemy initialization here
admin = Admin(app, name='Administrator', template_mode='bootstrap3')
admin.add_view(ModelView(User, db.session))
admin.add_view(ModelView(CTFSubSystems, db.session))