'use strict'
const switcher = document.querySelector('.btn');

// function swapStyleSheet(sheet) {
//     if( document.getElementById('pagestyle').getAttribute('href')== "style.css") {
//         document.getElementById('pagestyle').setAttribute('href', sheet);
//     } else{
//         document.getElementById('pagestyle').setAttribute('href', "style.css");
//     }
// }

var bool = 0;

if(!(window.localStorage.getItem('bool')==0 || window.localStorage.getItem('bool')==1)){
    window.localStorage.setItem('bool', 1);
}
var current = document.getElementsByTagName('link');

if(window.localStorage.getItem('bool')==0){
    current[0].href='cooler.css';
} else {
    current[0].href='style.css';
}

function swapStyleSheet(){
    var current = document.getElementsByTagName('link');

    if(window.localStorage.getItem('bool')==0){
        current[0].href='style.css';
        window.localStorage.setItem('bool', 1);
    }else{
        current[0].href='cooler.css';
        window.localStorage.setItem('bool', 0);
    }
}

switcher.addEventListener('click', function() {
    document.body.classList.toggle('theme');

    var className = document.body.className;

    if(className == "theme") {
        this.textContent = "Light Mode";
    }
    else {
        this.textContent = "Dark Mode";
    }
    console.log('current class name: ' + className);
});