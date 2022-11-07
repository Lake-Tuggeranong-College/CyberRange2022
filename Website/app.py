# This manages the whole Flask project, creating the different blueprints required
# The first is for the Game management sub-system
# The second is for the missile_launcher Launching sub-system website.
# They're split up, to make it easier to maintain.


from flask import Flask, render_template
from config import Config
from flask_sqlalchemy import SQLAlchemy
from flask_migrate import Migrate
from flask_login import LoginManager


# Configure app variables.

app = Flask(__name__)
app.config.from_object(Config)
# app.config['FLASK_ADMIN_SWATCH'] = 'cerulean'
db = SQLAlchemy(app)
migrate = Migrate(app, db)
login = LoginManager(app)
login.login_view = 'login'

# admin = Admin(app, name='Administrator', template_mode='bootstrap3')



from bp_missile_launcher import missile_launcher_blueprint
from bp_game_management import game_management_blueprint

app.register_blueprint(missile_launcher_blueprint)
app.register_blueprint(game_management_blueprint)


if __name__ == '__main__':
    app.run()



@app.route('/humans.txt')
def humans():
    return render_template("humans.txt", title="Humans")
