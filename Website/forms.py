from flask_wtf import FlaskForm
from wtforms import StringField, PasswordField, BooleanField, SubmitField, IntegerField
from models import User
from wtforms.validators import ValidationError, DataRequired, Email, EqualTo


class LoginForm(FlaskForm):
    username = StringField('Username', validators=[DataRequired()])
    password = PasswordField('Password', validators=[DataRequired()])
    remember_me = BooleanField('Remember Me')
    submit = SubmitField('Sign In')


class RegistrationForm(FlaskForm):
    name = StringField('Name', validators=[DataRequired()])
    username = StringField('Username', validators=[DataRequired()])
    email = StringField('Email', validators=[DataRequired(), Email()])
    password = PasswordField('Password', validators=[DataRequired()])
    password2 = PasswordField('Repeat Password', validators=[DataRequired(), EqualTo('password')])
    # current_score = StringField('Current Score', validators=[DataRequired()])
    submit = SubmitField('Register')

    def validate_username(self, username):
        user = User.query.filter_by(username=username.data).first()
        if user is not None:
            raise ValidationError('Please use a different username.')

    def validate_email(self, email):
        user = User.query.filter_by(email=email.data).first()
        if user is not None:
            raise ValidationError('Please use a different email address.')


class CTFSubsystemForm(FlaskForm):
    title = StringField("Title", validators=[DataRequired()])
    description = StringField("Description", validators=[DataRequired()])
    score = IntegerField("Score", validators=[DataRequired()])
    code = StringField('Passcode', validators=[DataRequired()])
    submit = SubmitField('Register')


class ClaimSubsystemForm(FlaskForm):
    checkbox = BooleanField('Item')
    submit = SubmitField('Claim')


class EditUserForm(FlaskForm):
    username = StringField('Username', validators=[DataRequired()])
    name = StringField('Name', validators=[DataRequired()])
    email = StringField('Email', validators=[DataRequired(), Email()])
    submit = SubmitField('Update User Details')


class ResetPasswordForm(FlaskForm):
    new_password = StringField('New Password', validators=[DataRequired()])
    submit = SubmitField('Submit')


class ClaimForm(FlaskForm):
    passcode = StringField('Passcode', validators=[DataRequired()])
    submit = SubmitField('Claim')

class ResetSubsystemsForm(FlaskForm):
    submit = SubmitField('Reset')
