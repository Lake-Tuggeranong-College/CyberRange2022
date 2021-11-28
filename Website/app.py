from flask import Flask, url_for
from flask import render_template, redirect, flash, request, Markup
from config import Config
from flask_sqlalchemy import SQLAlchemy
from flask_migrate import Migrate
from flask_login import LoginManager
from flask_login import logout_user
from flask_login import login_required
from sqlalchemy import text
from werkzeug.security import check_password_hash

# Configure app variables.

app = Flask(__name__)
app.config.from_object(Config)
db = SQLAlchemy(app)
migrate = Migrate(app, db)
login = LoginManager(app)
login.login_view = 'login'

from flask_login import current_user, login_user
from models import User, CTFSubSystems, Order
from forms import LoginForm, RegistrationForm, CTFSubsystemForm, ClaimSubsystemForm, EditUserForm, ResetPasswordForm, \
    ClaimForm, ResetSubsystemsForm


@app.route('/')
def main_page():
    title = "Home"
    return render_template('index.html', pagetitle=title, user=current_user)


@app.route('/login', methods=['GET', 'POST'])
def login():
    if current_user.is_authenticated:
        return redirect(url_for('main_page'))
    form = LoginForm()
    if form.validate_on_submit():
        user = User.query.filter_by(username=form.username.data).first()
        if user is None or not user.check_password(form.password.data):
            return redirect(url_for('login'))
        login_user(user, remember=form.remember_me.data)
        return redirect(url_for('main_page'))
    return render_template('login.html', title='Sign In', form=form, user=current_user)


@app.route('/logout')
def logout():
    logout_user()
    return redirect(url_for('main_page'))


@app.route('/user', )
@login_required
def user_details():
    return render_template("user.html", title="User Details", user=current_user)


@app.route('/register', methods=['GET', 'POST'])
def register():
    if current_user.is_authenticated:
        return redirect(url_for('main_page'))
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
        return redirect(url_for('login'))
    return render_template('register.html', title='Register', form=form, user=current_user)


@app.route('/registercharacter', methods=['GET', 'POST'])
@login_required
def registerCTFSubsystem():
    form = CTFSubsystemForm()
    if form.validate_on_submit():
        newSubSystem = CTFSubSystems(title=form.title.data, description=form.description.data, score=form.score.data,
                                     Owner="None")
        newSubSystem.set_passcode(form.code.data)
        db.session.add(newSubSystem)
        db.session.commit()
        flash('Congratulations, you have registered a new character!')
        return redirect(url_for('login'))
    return render_template('registersubsystem.html', title='Register Character', form=form, user=current_user)


@app.route('/secret', methods=['GET', 'POST'])
@login_required
def claimsubsystem():
    form = ClaimSubsystemForm()
    if form.validate_on_submit():
        print("submit")
        selected_subsystem = request.form.getlist("CTFSubSystems")
        print(selected_subsystem)

        for claimedsubsystems in selected_subsystem:
            claim_new_subsystem = Order(current_user.id, claimedsubsystems)
            db.session.add(claim_new_subsystem)
            flash("Ordered Product: {}".format(claim_new_subsystem.subsystemid))

        print("commit")
        db.session.commit()
        if len(selected_subsystem) > 0:
            flash("Subsystem Claimed")
        else:
            flash("Nothing selected. Please select one or more Subsystems to claim.")
        return redirect(url_for('main_page'))

    subsystems = text('select * from ctf_sub_systems')
    result = db.engine.execute(subsystems)

    return render_template('secret.html', pagetitle='Claim a Character', products=result, user=current_user, form=form)


@app.route('/edit_user/<userid>', methods=['GET', 'POST'])
@login_required
def edit_User(userid):
    form = EditUserForm()
    user = User.query.filter_by(id=userid).first()
    if form.validate_on_submit():
        user.update_details(form.username.data, form.name.data, form.email.data)
        db.session.commit()
        print("User Updated : {}".format(user))
        flash("User Reset")
        return redirect(url_for('user_details'))

    form.username.data = user.username
    form.email.data = user.email
    form.name.data = user.name
    return render_template('edit-user.html', title='Reset Password', form=form, user=user)


@app.route('/report/listallusers')
def display_users():
    sql = text('select username, id from user')
    result = db.engine.execute(sql)
    users = []
    html_output = Markup(
        "<div class=\"container-fluid table table-hover text-centered font-color\"><div class = \"row\"><div class=\"col-sm-3 "
        "font-weight-bold\">ID</div><div class=\"col-sm-3 font-weight-bold\">User Name</div><div class=\"col-sm-3 "
        "font-weight-bold\">Reset Password</div><div class=\"col-sm-3 font-weight-bold\">Edit User "
        "Details</div></div>")
    for row in result:
        users.append(row)
    print(users)
    user_counter = 1
    for index, user in enumerate(users):

        if index % 2 == 0:
            html_output = Markup(
                "{}<div class = \"row cell1 font-color\"><div class=\"col-sm-3\">{}</div> <div class=\"col-sm-3\">{}</div><div "
                "class=\"col-sm-3\"><a href=\"/reset_password/{}\">Reset Password</a></div> <div "
                "class=\"col-sm-3\"><a href=\"/edit_user/{}\">Edit User Details</a></div></div>".format(
                    html_output, user_counter, user[0], user[1], user[1]))
        else:
            html_output = Markup(
                "{}<div class = \"row cell2 font-color\"><div class=\"col-sm-3\">{}</div> <div class=\"col-sm-3\">{}</div><div "
                "class=\"col-sm-3\"><a href=\"/reset_password/{}\">Reset Password</a></div><div class=\"col-sm-3\"><a "
                "href=\"/edit_user/{}\">Edit User Details</a></div></div>".format(
                    html_output, user_counter, user[0], user[1], user[1]))
        user_counter = user_counter + 1

    html_output = Markup("{}</tbody></table>".format(html_output))
    print(html_output)

    return render_template('list-users.html', Title='List of Users', data=html_output, user=current_user)


if __name__ == '__main__':
    app.run()


@app.route('/report/stocklevels')
def all_user_details():
    sql = text('select name, username, email, id from user')
    result = db.engine.execute(sql)
    users = []
    html_output = Markup(
        "<div class=\"container-fluid table table-hover text-centered font-color\"><div class = \"row\"><div class=\"col-sm-3 "
        "font-weight-bold\">ID</div><div class=\"col-sm-3 font-weight-bold\">Name</div><div class=\"col-sm-3 "
        "font-weight-bold\">Username</div><div class=\"col-sm-3 font-weight-bold\">Email</div></div>")
    for row in result:
        users.append(row)
    print(users)
    user_counter = 1
    for index, user in enumerate(users):

        if index % 2 == 0:
            html_output = Markup(
                "{}<div class = \"row cell1 font-color\"><div class=\"col-sm-3\">{}</div> <div class=\"col-sm-3\">{}</div>"
                "<div class=\"col-sm-3\">{}</div> <div class=\"col-sm-3\">{}</div></div>".format(
                    html_output, user_counter, user[0], user[1], user[2]))
        else:
            html_output = Markup(
                "{}<div class = \"row cell2 font-color\"><div class=\"col-sm-3\">{}</div> <div class=\"col-sm-3\">{}</div>"
                "<div class=\"col-sm-3\">{}</div> <div class=\"col-sm-3\">{}</div></div>".format(
                    html_output, user_counter, user[0], user[1], user[2]))
        user_counter = user_counter + 1

    html_output = Markup("{}</tbody></table>".format(html_output))
    print(html_output)

    return render_template('user-details.html', Title='Users Details', data=html_output, user=current_user)


@app.route('/report/u_ranked')
@login_required
def ranked_users():
    ranked = text('select username, current_score from user where active_player')
    result = db.engine.execute(ranked)
    users = []
    html_output = Markup(
        "<div class=\"container-fluid table table-hover text-centered user-size\"><div class = \"row\">"
        "<div class=\"col-sm-3 \"font-weight-bold\"></div><div "
        "class=\"col-sm-3 font-weight-bold\">Username</div><div class=\"col-sm-3 "
        "font-weight-bold\">Score</div><div class=\"col-sm-3 "
        "font-weight-bold\"></div></div> "
    )

    for row in result:
        users.append(row)
    print(users)
    # user_counter = 1
    for index, user in enumerate(users):

        if index % 2 == 0:
            html_output = Markup("{}<div class = \"row cell1 user-size\"> "
                                 "<div class=\"col-sm-3\"></div><div class=\"col-sm-3\">{}</div><div "
                                 "class=\"col-sm-3\">{}</div><div class=\"col-sm-3\"></div> "
                                 "</div>".format(html_output, user[0], user[1]))
        else:
            html_output = Markup("{}<div class = \"row cell2 user-size\"> "
                                 "<div class=\"col-sm-3\"></div><div class=\"col-sm-3\">{}</div><div "
                                 "class=\"col-sm-3\">{}</div></div><div class=\"col-sm-3\"> "
                                 "</div>".format(html_output, user[0], user[1]))
        # user_counter = user_counter + 1

    html_output = Markup("{}</div></section></tbody><table>".format(html_output))
    print(html_output)

    return render_template("user-ranks.html", Title="Scoreboard", data=html_output, user=current_user)


@app.route('/reset_password/<userid>', methods=['GET', 'POST'])
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
        return redirect(url_for('user_details'))

    return render_template('reset-password.html', title='Reset Password', form=form, user=user)


@app.route('/claimcharacter', methods=['GET', 'POST'])
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
                    flash("You have already claimed this character")
                else:
                    update_system.claim()
                    flash("{} - Character claimed".format(update_system.title))
                    user = User.query.filter_by(username=current_user.username).first()
                    score = user.current_score + update_system.score
                    print("score is: {}".format(score))
                    update_system.Owner = current_user.username
                    user.current_score = score

        db.session.commit()

    return render_template('claimsubsystem.html', pagetitle='Claim a Character', form=form, user=current_user)


@app.route('/reset', methods=['GET', 'POST'])
def reset_subsystems():
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

    return render_template('reset.html', pagetitle='Reset Characters', form=form, user=current_user)


@app.route('/report/dashboard')
def dashboard():
    subsystems = text('select title, Owner, Status from ctf_sub_systems')
    result = db.engine.execute(subsystems)
    subsystem_list = []

    for row in result:
        subsystem_list.append(row)
    html_output = Markup(
        "<div class=\"container-fluid table table-hover text-centered font-color\"><div class = \"row\>")

    for systems in subsystem_list:
        html_output = Markup("{}<div class=\"col-sm-1\">{}</div>".format(html_output, systems.title))
    html_output = Markup(
        "{}</div>".format(html_output))

    print(html_output)
    print(subsystem_list)

    return render_template('dashboard.html', Title='Subsystem Dashboard', data=html_output, user=current_user, subsystems=subsystem_list)




@app.route('/humans.txt')
def humans():
    return render_template("humans.txt", title="Humans")
