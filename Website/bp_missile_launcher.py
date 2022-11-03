# All the code/routes for the missile launcher will be stored here.

from flask import Blueprint, render_template
from flask_login import current_user, login_required
# import usb.core
import os

missile_launcher_blueprint = Blueprint('missile_launcher_blueprint', __name__)

@missile_launcher_blueprint.route('/missile_launcher/index')
@missile_launcher_blueprint.route('/missile_launcher/')
@missile_launcher_blueprint.route('/missile_launcher')
def missile_launcher_main_page():
    return render_template('missile_launcher/index.html', user=current_user)


# @missile_launcher_blueprint.route('/login', methods=['GET', 'POST'])
# def login():
#     if current_user.is_authenticated:
#         return redirect(url_for('main_page'))
#     form = LoginForm()
#     if form.validate_on_submit():
#         user = User.query.filter_by(username=form.username.data).first()
#         if user is None or not user.check_password(form.password.data):
#             # flash('Invalid username or password')
#             return redirect(url_for('main_page'))
#         login_user(user, remember=form.remember_me.data)
#         return redirect(url_for('fire'))
#     return render_template('userLogin.html', title='Sign In', form=form)

@missile_launcher_blueprint.route('/missile_launcher/fire')
def fire():
    return render_template('missile_launcher/fire.html', user=current_user)


@missile_launcher_blueprint.route(
    '/missile_launcher/tgwjkdfhaasdnbfmasdfvgkasd')  # This will run if the letters included here are put in the url.
@login_required
def fire_missile():  # This is the fire function controls the missile fire
    """This function is what allows the missile to fire,
    it also detects the missile launcher from the USB."""

    if os.uname()[4][:3] == 'arm':
        dev = usb.core.find(idVendor=0x2123, idProduct=0x1010)  # Finds the missile launcher via usb.
        if dev is None:  # If it can't find the missile launcher.
            raise ValueError('Launcher not found.')  # This will be printed if the missile launcher cannot be found.
        # Determines whether there is a Kernel driver associated with the missile launcher.
        if dev.is_kernel_driver_active(0) is True:
            dev.detach_kernel_driver(0)  # If there is a Kernal driver associated with the missile launcher it detaches it.
            dev.set_configuration()  # The code will then configure the USB missile launcher.

        # This is what allows the missile to fire.
        dev.ctrl_transfer(0x21, 0x09, 0, 0, [0x02, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00])
        return render_template('missile_launcher/fired.html', output="Fired", user=current_user)

    else:
        return render_template('missile_launcher/fired.html', output="This functionality only works on a raspberry pi", user=current_user)
