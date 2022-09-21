# All the code/routes for the game management will be stored here.
from flask import Flask, url_for, render_template, redirect, flash, request, Markup, Blueprint
from flask_login import current_user, login_user
from flask_login import LoginManager, logout_user, login_required
from forms import LoginForm, RegistrationForm, CTFSubsystemForm, ClaimSubsystemForm, EditUserForm, ResetPasswordForm, \
    ClaimForm, ResetSubsystemsForm, ResetModuleCodeForm
from models import User, CTFSubSystems
from sqlalchemy import text
from app import db
from werkzeug.security import check_password_hash, generate_password_hash

game_management_blueprint = Blueprint('game_management_blueprint', __name__)


@game_management_blueprint.route('/index')
@game_management_blueprint.route('/')
def game_main_page():
    title = "Home"
    return render_template('index.html', pagetitle=title, user=current_user)


@game_management_blueprint.route('/login', methods=['GET', 'POST'])
def game_user_login():
    if current_user.is_authenticated:
        return redirect(url_for('game_management_blueprint.game_main_page'))
    form = LoginForm()
    if form.validate_on_submit():
        user = User.query.filter_by(username=form.username.data).first()
        if user is None or not user.check_password(form.password.data):
            flash("Username or password is incorrect. Please check and try again.")
            return redirect(url_for('game_management_blueprint.game_user_login'))
        login_user(user, remember=form.remember_me.data)
        return redirect(url_for('game_management_blueprint.game_main_page'))
    return render_template('userLogin.html', title='Sign In', form=form, user=current_user)


@game_management_blueprint.route('/logout')
def logout():
    logout_user()
    return redirect(url_for('game_management_blueprint.game_main_page'))


@game_management_blueprint.route('/user', )
@login_required
def game_user_details():
    return render_template("user.html", title="User Details", user=current_user)


@game_management_blueprint.route('/registerUser', methods=['GET', 'POST'])
def register_user():
    if current_user.is_authenticated:
        return redirect(url_for('game_management_blueprint.game_main_page'))
    form = RegistrationForm()
    if form.validate_on_submit():
        print("test")
        user = User(name=form.name.data, username=form.username.data, email=form.email.data, current_score=0,
                    is_administrator=False)
        print(user)
        user.set_password(form.password.data)
        db.session.add(user)

        db.session.commit()
        flash('Congratulations, you are now a registered user!')
        return redirect(url_for('game_management_blueprint.game_user_login'))
    return render_template('userRegister.html', title='Register', form=form, user=current_user)


@game_management_blueprint.route('/registerModule', methods=['GET', 'POST'])
@login_required
def register_module():
    form = CTFSubsystemForm()
    if form.validate_on_submit():
        newSubSystem = CTFSubSystems(title=form.title.data, description=form.description.data, score=form.score.data,
                                     Owner="None", active=1)
        newSubSystem.set_passcode(form.code.data)
        db.session.add(newSubSystem)
        db.session.commit()
        flash('Congratulations, you have registered a new Module!')
        return redirect(url_for('game_management_blueprint.game_user_login'))
    return render_template('registerModule.html', title='Register Module', form=form, user=current_user)


@game_management_blueprint.route('/edit_user/<userid>', methods=['GET', 'POST'])
@login_required
def edit_user(userid):
    form = EditUserForm()
    user = User.query.filter_by(id=userid).first()
    if form.validate_on_submit():
        user.update_details(form.username.data, form.name.data, form.email.data)
        db.session.commit()
        print("User Updated : {}".format(user))
        flash("User Reset")
        return redirect(url_for('game_management_blueprint.game_user_details'))

    form.username.data = user.username
    form.email.data = user.email
    form.name.data = user.name
    return render_template('userEdit.html', title='Reset Password', form=form, user=user)


@game_management_blueprint.route('/report/listallusers')
def display_users():
    sql = text('select username, id from user')
    result = db.engine.execute(sql)
    users = []

    for row in result:
        users.append(row)

    return render_template('userList.html', Title='List of Users', data=users, user=current_user)


@game_management_blueprint.route('/report/allUserDetails')
def all_user_details():
    sql = text('select name, username, email, id from user')
    result = db.engine.execute(sql)
    users = []

    for row in result:
        users.append(row)
    print(users)

    return render_template('userDetails.html', Title='Users Details', data=users, user=current_user)


@game_management_blueprint.route('/report/u_ranked')
@login_required
def ranked_users():
    ranked = text('select username, current_score from user where active_player')
    result = db.engine.execute(ranked)
    users = []

    for row in result:
        users.append(row)
    users.sort(key=lambda x: x[1], reverse=True)

    return render_template("userRanks.html", Title="Scoreboard", user_data=users, user=current_user)


@game_management_blueprint.route('/reset_password/<userid>', methods=['GET', 'POST'])
@login_required
def reset_user_password(userid):
    form = ResetPasswordForm()
    user = User.query.filter_by(id=userid).first()
    if form.validate_on_submit():
        print("Resetting Password:{}".format(form.new_password.data))

        user.set_password(form.new_password.data)
        db.session.commit()
        print("done")
        flash('Password has been reset for user {}'.format(user.username))
        return redirect(url_for('game_management_blueprint.game_user_details'))

    return render_template('resetPassword.html', title='Reset Password', form=form, user=user)


@game_management_blueprint.route('/reset_passcode/<moduleid>', methods=['GET', 'POST'])
@login_required
def reset_module_passcode(moduleid):
    form = ResetModuleCodeForm()
    module = CTFSubSystems.query.filter_by(subsystemid=moduleid).first()
    if form.validate_on_submit():
        module.set_passcode(form.new_passcode.data)
        db.session.commit()
        print("done")
        flash('Password has been reset for module {}'.format(module.title))
        return redirect(url_for('game_management_blueprint.game_main_page'))

    return render_template('resetPasscode.html', title='Reset Passcode', form=form, user=current_user, module=module)


@game_management_blueprint.route('/claimModule', methods=['GET', 'POST'])
def claim():
    form = ClaimForm()
    if form.validate_on_submit():
        code = form.passcode.data

        sql = text('select * from ctf_sub_systems')
        result = db.engine.execute(sql)

        systems = []

        for index, system in enumerate(result):
            print(check_password_hash(system.Code, code))
            if check_password_hash(system.Code, code):
                # update status to true
                # result = db.engine.execute(text('Update ctf_sub_systems SET Status = TRUE WHERE subsystemid=:subid'), subid=index)
                print(system.title)
                update_system = CTFSubSystems.query.filter_by(title=system.title).first()
                print(update_system.title)
                if update_system.status:
                    flash("You have already claimed this Module")
                else:
                    update_system.claim()
                    flash("{} - Module claimed".format(update_system.title))
                    user = User.query.filter_by(username=current_user.username).first()
                    score = user.current_score + update_system.score
                    print("score is: {}".format(score))
                    update_system.Owner = current_user.username
                    user.current_score = score

        db.session.commit()

    return render_template('claimModule.html', pagetitle='Claim a Module', form=form, user=current_user)


@game_management_blueprint.route('/reset', methods=['GET', 'POST'])
def reset_game():
    form = ResetSubsystemsForm()
    if form.validate_on_submit():
        sql = text('select * from ctf_sub_systems')
        result = db.engine.execute(sql)

        for index, system in enumerate(result):
            reset_subsystem = CTFSubSystems.query.filter_by(title=system.title).first()
            print(reset_subsystem.title)
            if reset_subsystem.status:
                reset_subsystem.reset()
                reset_subsystem.Owner = 'None'
                reset_subsystem.status = False
                flash("You have reset - {}".format(reset_subsystem.title))

        db.session.commit()

    return render_template('reset.html', pagetitle='Reset Game', form=form, user=current_user)


@game_management_blueprint.route('/report/dashboard')
def dashboard():
    subsystems = text('select title, Owner, Status from ctf_sub_systems')
    result = db.engine.execute(subsystems)
    subsystem_list = []

    for row in result:
        subsystem_list.append(row)

    return render_template('dashboard.html', Title='Subsystem Dashboard', user=current_user, subsystems=subsystem_list)


@game_management_blueprint.route('/module/<moduleid>', methods=["GET", "POST"])
@login_required
def module_information(moduleid):
    # Steps needed.
    # 1. Load Specific record (moduleid) from table
    # 2. Display relevant information
    # 3. Include form to submit and check code.
    module_info = CTFSubSystems.query.filter_by(subsystemid=moduleid).first()
    form = ClaimForm()
    if form.validate_on_submit():
        if check_password_hash(module_info.Code, form.passcode.data):
            current_user.current_score = current_user.current_score + module_info.score
            msg = "Success! You entered the correct code!. You gained" + str(
                module_info.score) + " points. You now have " + str(current_user.current_score) + " points"
            # flash("Success! You entered the correct code!.")
            flash(msg)
            db.session.commit()
        else:
            flash("Incorrect Code. Try again.")

    return render_template('moduleInformation.html', Title='moduleInformation', user=current_user, module=module_info,
                           form=form)


@game_management_blueprint.route('/module', methods=["GET", "POST"])
@login_required
def module_list():
    ctf_modules = CTFSubSystems.query.filter_by(active=1).all()  # Only show 'active' modules.
    return render_template('moduleList.html', Title='Available Modules', user=current_user, modules=ctf_modules)
