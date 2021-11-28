# All the code/routes for the game management will be stored here.


from flask import Blueprint

game_management_blueprint = Blueprint('game_management_blueprint', __name__)

@game_management_blueprint.route('/game/index')
@game_management_blueprint.route('/game/')
@game_management_blueprint.route('/game')
def game_main_page():
    return "Game Management Test page"