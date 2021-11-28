# All the code/routes for the missile launcher will be stored here.

from flask import Blueprint

missile_launcher_blueprint = Blueprint('missile_launcher_blueprint', __name__)

@missile_launcher_blueprint.route('/missile_launcher/index')
@missile_launcher_blueprint.route('/missile_launcher/')
@missile_launcher_blueprint.route('/missile_launcher')
def missile_launcher_main_page():
    return "Missile Launcher Test page"