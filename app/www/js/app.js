var app = {
    user: {},
    debug: false,
    icon: "img/spote32.jpg",
    initialize: function () {
        console.log('app initialize');
        document.addEventListener('deviceready', this.onDeviceReady.bind(this), false);
    },

    onDeviceReady: function () {
        debug.log('device ready', 'success');
        // screen.orientation.lock('landscape');
        app.bindEvents();

        controller.initialize();
        $('#controls').hide();
        $('#spotlink').hide(); 
        $('#sleep').hide();
        $('#spotlink').show(); 
        
        bluetooth.initialize();

        $('#device').on('click', '#refreshDeviceList', function (e) {
            bluetooth.refreshDeviceList();
        });

        var previousConnectedDevice = storage.getItem('connectedDevice');
        console.log("previousConnectedDevice " + previousConnectedDevice);

        if (previousConnectedDevice != undefined)
        {
            var html = '<ons-list-item id="' + previousConnectedDevice.id +
                        '" data-device-id="' + previousConnectedDevice.id + 
                        '" data-device-name="' + previousConnectedDevice.name + '" tappable>' +
                '<div class="left"><img class="list-item__thumbnail" src="'+ app.icon +'"></div>' +
                '<div class="center">' +
                    '<span class="list-item__title">' + previousConnectedDevice.name + '</span>' +
                    '<span class="list-item__subtitle">' + previousConnectedDevice.id + '</span>' +
                '</div>' +
                '<div class="right"><ons-progress-circular indeterminate></ons-progress-circular></div>' +
                '</ons-list-item>';
            $('#spot-devices-div').append(html);
            
        } else {
            // storage.removeItem('connectedDevice');
            // $('#spotlink').show();
        }
        $('#spotlink').show(); 
    },

    bindEvents: function () {
        // disable device buttons to have full screen view
        document.addEventListener("pause", app.onDevicePause, false);
        document.addEventListener("resume", app.onDeviceResume, false);
        document.addEventListener("menubutton", app.onMenuKeyDown, false);

        $('#spot-devices-div').on('click', 'ons-list-item', function (e) {
            if($(this).attr("data-device-conn") == "idle" || $(this).attr("data-device-conn") == "error")
            {
                var html = '<ons-list-item id="' + $(this).attr("data-device-id") +
                             '" data-device-id="' + $(this).attr("data-device-id") +
                             '" data-device-name="' + $(this).attr("data-device-name") +
                              '" data-device-conn="connecting" tappable>' +
                            '<div class="left"><img class="list-item__thumbnail" src="'+ app.icon +'"></div>' +
                            '<div class="center">' +
                                '<span class="list-item__title">' + $(this).attr("data-device-name") + '</span>' +
                                '<span class="list-item__subtitle">' + $(this).attr("data-device-id") + '</span>' +
                            '</div>' +
                            '<div class="right"><ons-progress-circular indeterminate></ons-progress-circular></div>' +
                            '</ons-list-item>';
                
                    $(this).remove();
                    $('#spot-devices-div').append(html);

                    bluetooth.connectDevice($(this).attr("data-device-id"), $(this).attr("data-device-name"));
            }
        });

        document.getElementById('ble_button').onclick = function() {
            storage.removeItem('connectedDevice');
            bluetooth.disconnectDevice(false);
        }
        document.getElementById('sleep_btn').onclick = app.sleep_btn;
        document.getElementById('wakeup_btn').onclick = app.wakeup_btn;
        document.getElementById('bark_btn').onclick = app.bark_btn;
        document.getElementById('hifi_btn').onclick = app.hifi_btn;
        document.getElementById('pee_btn').onclick = app.pee_btn;
    },

    sleep_btn: function() {
        bluetooth.sendOrientation(0, 0, 0, -40, -170, 0);
    },
    wakeup_btn: function() {
        bluetooth.sendOrientation(0, 0, 0, 0, 0, 0);
    },
    bark_btn: function() {
        //TODO: play this on ESP32
        var my_media = new Media("/android_asset/www/bark.mp3",
            // success callback
            function() {
                console.log("playAudio():Audio Success");
            },
            // error callback
            function(err) {
                console.log("playAudio():Audio Error: "+err);
        });
    
        // Play audio
        my_media.play();
    },
    hifi_btn: function() {
        //TODO:
        ons.notification.toast('Not yet!', { timeout: 1000, animation: 'fall' });
    },
    pee_btn: function() {
        //TODO:
        ons.notification.toast('Defenitely Not yet!', { timeout: 1000, animation: 'fall' });
    },

    onDevicePause: function () {
        debug.log('in pause');
    },
    onDeviceResume: function () {
        debug.log('out of pause');
        bluetooth.refreshDeviceList();
    },
    onMenuKeyDown: function () {
        debug.log('menubuttonpressed');
    },
    onError: function (error) {
        debug.log(JSON.stringify(error), 'error');
    }
};

app.initialize();