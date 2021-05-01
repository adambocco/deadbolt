const axios = require('axios')

axios.get('http://192.168.2.222/5/on')


function sendPinCode() {
    let XHR = new XMLHttpRequest();
    XHR.onload = () => {
        let body = JSON.parse(this.responseText);

        if (this.status == 200) {
            if (body['response'] == "Unlocked") {
                statusSection.innerHTML = body['response'];
                setTimeout(()=> {
                    statusSection.innerHTML = "Locked";
                },6000);
            }
        } else if (this.status == 400) {
            statusSection.innerHTML = body['response'];
        } else {
            statusSection.innerHTML = "Error";
        }
    }
    XHR.onerror = () => {alert("Server Error")};
    XHR.open("POST", "http://" + window.location.hostname + "/unlock");
    XHR.send(JSON.stringify({ "pin" :  displayPin.innerText}));
}