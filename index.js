
console.log(__dirname);

var kkDeviceApi = require(__dirname + '/deviceSwitch');
var Accessory, Service, Characteristic, UUIDGen;
var that;  //指向SamplePlatform 初始化内部this
var onDeviceMacAry = []; //记录上线过的设备，已经设置了一遍属性，就不要再添加了

module.exports = function(homebridge) {
  console.log("homebridge API version: " + homebridge.version);
  // Accessory must be created from PlatformAccessory Constructor
  Accessory = homebridge.platformAccessory;

  // Service and Characteristic are from hap-nodejs
  Service = homebridge.hap.Service;
  Characteristic = homebridge.hap.Characteristic;
  UUIDGen = homebridge.hap.uuid;
  
  // For platform plugin to be considered as dynamic platform plugin,
  // registerPlatform(pluginName, platformName, constructor, dynamic), dynamic must be true

    homebridge.registerPlatform("homebridge-konkePlatform", "KonkePlatform", KonkePlatform, true);
};

// Platform constructor
// config may be null
// api may be null if launched from old homebridge version
function KonkePlatform(log, config, api) {
  log("SamplePlatform Init");
  this.log = log;
  this.config = config;
  this.kkAccessories = [];
  that = this;
  if (api) {
      // Save the API object as plugin needs to register new accessory via this object
      this.api = api;
      // Listen to event "didFinishLaunching", this means homebridge already finished loading cached accessories.
      // Platform Plugin should only register new accessory that doesn't exist in homebridge after this event.
      // Or start discover new accessories.
      this.api.on('didFinishLaunching', function() {
          this.log("DidFinishLaunching");
          kkDeviceApi.DeviceControl.DeviceDiscovery();
      }.bind(this));
  }else {
      this.log('init error');
  }
}
/*---------------为KonkePlatform添加方法------------*/
KonkePlatform.prototype = {
    //删除设备，根据devMac
    removeDevice: function (devMac) {
        for (var index in that.kkAccessories) {
            var obj = that.kkAccessories[index];
            if (devMac === obj.context.did) {
                console.log('取消设备=', obj);
                that.api.unregisterPlatformAccessories("homebridge-konkePlatform", "KonkePlatform", [obj]);
                break;
            }
        }
    },

    //发现设备
    onDevice: function (devInfo) {
        var that2 = this;   //指向当前，回调函数需要使用
        //判断设备类型
        var serviceObj = this.decideDevType(devInfo['device_type']);
        if (serviceObj === null) {
            console.log('未知设备000');
            return;
        }
        var uuid;
        var name;
        var newAccessory = null;
        var devExit = 0;
        var kkService = null;
        for (var index in that.kkAccessories) {
            var obj = that.kkAccessories[index];
            if (obj.context.did === devInfo['device_mac']) {
                devExit = 1;
                newAccessory = obj;
                break;
            }
        }
        if (devExit) {
            console.log("已存在设备cached accessory: " + newAccessory.context.did);
            kkService = newAccessory.getService(serviceObj);
        }else {
            uuid = UUIDGen.generate(devInfo['device_mac']);
            name = devInfo['device_mac'].substring(devInfo['device_mac'].length-6);
            newAccessory = new Accessory(name, uuid);
            newAccessory.context.did = devInfo['device_mac'];
            newAccessory.context.model = devInfo['device_type'];
            kkService = new serviceObj(name);
            console.log("新创建设备newAccessory: " + newAccessory.context.did);
        }
        devInfo.accessory = newAccessory;
        // onDeviceMacAry.push(devInfo['device_mac']);
        var switchValue = kkService.getCharacteristic(Characteristic.On);
        console.log('设备开关状态=', switchValue);
        switchValue.on('set', function(value, callback) {
                console.log('设置属性===', value);
                that2.controlDevice(devInfo['device_mac'], "power", value, callback);
        });
        if (devExit) {
            if (this.isSwitch(devInfo['device_type'])) {
                this.isSwithcGetCharacteristic(kkService, devInfo);
            }else if (this.isKlight(devInfo['device_type'])) {
                this.isKlightGetCharacteristic(kkService, devInfo);
            }
        }else {
            if (this.isSwitch(devInfo['device_type'])) {
                this.isSwithcAddCharacteristic(kkService, devInfo);
            }else if (this.isKlight(devInfo['device_type'])) {
                this.isKlightAddCharacteristic(kkService, devInfo);
            }
        }
        newAccessory.reachable = true;
        if (!devExit) {
            console.log('注册这个设备', newAccessory.context.did);
            newAccessory.addService(kkService, name);
            that.kkAccessories.push(newAccessory);
            that.api.registerPlatformAccessories("homebridge-konkePlatform", "KonkePlatform", [newAccessory]);
        }else {
            that.api.updatePlatformAccessories([newAccessory]);
        }
    },

    //外部会调用
    configureAccessory: function(accessory) {
        console.log('configureAccessory11', accessory.context.did);
        // accessory.updateReachability(false);
        accessory.reachable = true;
        accessory.on('identify', function(paired, callback) {
            console.log("identify ....");
        });
        that.kkAccessories.push(accessory);
    },

    //执行命令
    controlDevice: function(did, characteristic, value, callback) {
        console.log('控制设备=',did);
        var devInfo = kkDeviceApi.DeviceControl.SearchDeviceInfo(did);
        if (devInfo === null) {
            console.log("no device found for did: " + did);
            this.removeDevice(did);
            if (callback)
                callback();
            return;
        }
        switch(characteristic.toLowerCase()) {
            case 'identify':
                console.log("identfy....");
                // dev.setBlink();
                break;
            case 'power':
                console.log('power....');
                kkDeviceApi.DeviceControl.SetSwitch(did, value, function (result) {
                    if (callback)
                        callback();
                });
                break;
            case 'hue':
                // dev.setColor(value, dev.sat);
                break;
            case 'brightness':
                // dev.setBright(value);
                break;
            case 'saturation':
                // dev.setColor(dev.hue, value);
                break;
            default:
                break;
        }
    },

    decideDevType: function (devType) {
        if (devType === 'k2' ||
            devType === 'mini_w' ||
            devType === 'k2pro' ||
            devType === 'k2pro') {
            return Service.Switch;
        }else if (devType === 'klight'){
            return Service.Lightbulb;
        }else {
            return null;
        }
    },

    isSwitch: function (devType) {
        if (devType === 'k2' ||
            devType === 'mini_w' ||
            devType === 'k2pro' ||
            devType === 'k2pro') {
            return 1;
        }
        return 0;
    },

    isKlight: function (devType) {
        if (devType === 'klight') {
            return 1;
        }
        return 0;
    },

    isSwithcGetCharacteristic: function (service, devInfo) {

    },

    isKlightGetCharacteristic: function (lightService, devInfo) {
        var that2 = this;   //指向当前，回调函数需要使用
        lightService
            .getCharacteristic(Characteristic.Brightness)
            .on('set', function(value, callback) { that2.controlDevice(devInfo['devMac'], "brightness", value, callback);})
        // .value = dev.bright;
        lightService
            .getCharacteristic(Characteristic.Hue)
            .on('set', function(value, callback) { that2.controlDevice(devInfo['devMac'], "hue", value, callback);})
            // .value = dev.hue;
        lightService
            .getCharacteristic(Characteristic.Saturation)
            .on('set', function(value, callback) { that2.controlDevice(devInfo['devMac'], "saturation", value, callback);})
            // .value = dev.sat;
    },

    isSwithcAddCharacteristic: function (service, devInfo) {

    },

    isKlightAddCharacteristic: function (lightService, devInfo) {
        var that2 = this;   //指向当前，回调函数需要使用
        lightService
            .addCharacteristic(Characteristic.Brightness)
            .on('set', function(value, callback) { that2.controlDevice(devInfo['devMac'], "brightness", value, callback);})
            // .value = dev.bright;
        lightService
            .addCharacteristic(Characteristic.Hue)
            .on('set', function(value, callback) { that2.controlDevice(devInfo['devMac'], "hue", value, callback);})
            // .value = dev.hue;

        lightService
            .addCharacteristic(Characteristic.Saturation)
            .on('set', function(value, callback) { that2.controlDevice(devInfo['devMac'], "saturation", value, callback);})
            // .value = dev.sat;
    }
};
exports.KonkePlatform = KonkePlatform;

