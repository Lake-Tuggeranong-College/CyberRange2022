function check_pin_code() {
    var pin_entered, username_entered;
    //Get the value of input field with id="pin_code"
    pin_entered = document.getElementById("missilePassword").value;
    username_entered = document.getElementById("missileUsername").value;
    let text = "test"
    // Check to see if pin_entered is correct
    if ((pin_entered === "dictionary") && (username_entered === "root")) {
        text = "<div class='bg-success'><a href ='/missile_launcher/fire' class='text-white'>Press Here to Continue!</a></div>";
    } else {
        text = "<div class='bg-danger text-white'>Username and/or Password Incorrect</div>";
    }
    document.getElementById("fire_button").innerHTML = text;
}
