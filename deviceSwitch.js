/**
 * Created by kk on 2017/11/10.
 */
var ffi = require("ffi");
var ref = require('ref');
var Struct = require('ref-struct');
var homeKit = require(__dirname + '/index');

var path = require('path');

/*--------------全局数据参数-------------*/
var deviceInfoList;  //设备列表json对象
var deviceListAry = [];
var getDevListCount = 0;

/*---------------类型定义----------------*/
//发现设备类型
var startDiscoveryStruct = Struct({
    'pCallBack': 'pointer',
    'szBroadcastAddr': 'string'
});
var sdStruct = ref.refType(startDiscoveryStruct);

/*---------------回调方法----------------*/
//发现设备回调
var startDiscoveryCallback = ffi.Callback('void', ['string'],
    function(pcStr) {
        if (getDevListCount%3 === 0) {
            getDeviceInfoList();
        }
        getDevListCount++;
        console.log("pcStr: ", pcStr);
    });

/*---------------注册方法----------------*/
//sdk路径
var ffiPath = path.join(__dirname,'/libKonkeInterface');
console.log(ffiPath);

var libm = ffi.Library(ffiPath, {
    'StartDiscovery': ['void', [sdStruct]],
    'GetDevList': ['int', ['string']],
    'OperateDeviceRelay': ['string', ['string', 'string']]
});

/*---------------调用方法----------------*/
//1：发现设备方法
var sd = new startDiscoveryStruct();
sd.pCallBack = startDiscoveryCallback;
sd.szBroadcastAddr = '192.168.199.255';

var deviceDiscovery = function () {
    console.log('createThreadStart');
    var outSd = sd.ref();
    libm.StartDiscovery.async(outSd, function (err, res) {
        if (err) throw err;
        console.log("mycall returned " + res);
    });
};

//2：获取设备列表
var getDeviceInfoList = function () {
    console.log('getDeviceInfoList');
    var devInfoListStr = new Buffer(2048);
    devInfoListStr.fill('##');
    var result = libm.GetDevList(devInfoListStr);
    if (result === 0) {
        console.log('获取设备列表失败');
    }
    //这里'\u0000'找了好久，需要截取字符串才能转json对象
    var str = devInfoListStr.toString();
    var index = str.indexOf("\u0000");
    var devStr = str.substr(0, index);
    deviceInfoList = JSON.parse(devStr);
    console.log('json =====', deviceInfoList);

    var presentDevListAry = deviceInfoList['device_list'];
    dealWithDeviceList(presentDevListAry);
};

//3：查找设备
var searchDeviceInfo = function (devMac) {
    console.log('searchDeviceInfo');
    var devListAry = deviceInfoList['device_list'];
    for (var index in devListAry) {
        var obj = devListAry[index];
        if (obj['device_mac'] === devMac && obj['device_type'] !== 'konke_other') {
            return obj;
        }
    }
    return null;
};

//找到不存在设备，删除
var dealWithDeviceList = function (presentDevlist) {
    var addAry = [];
    for (var index in presentDevlist) {
          var obj = presentDevlist[index];
          if (deviceListAry.contains(obj) > 0) {
              console.log('1111111');
          }else {
              console.log('2222222');
              addAry.push(obj);
              addDevice(obj);
          }
    }
    var deleteAry = [];
    for (index in deviceListAry) {
          obj = deviceListAry[index];
          if (presentDevlist.contains(obj) > 0) {
              console.log('3333333');
          }else {
              console.log('4444444');
              deleteAry.push(obj);
              removeDevice(obj);
          }
    }
    deviceListAry = presentDevlist;
};

//4：设置开关
var setSwitch = function (devMac, value, callBack) {
    var openClose = value ? 'open' : 'close';
    console.log('开关setSwitch', devMac, 'value', openClose);
    var result = libm.OperateDeviceRelay(devMac, openClose);
    console.log('开关setSwitch-result=', result);
    callBack (result);
};

/*---------------homekit方法------------*/
var uploadDevice = function () {
    homeKit.KonkePlatform.prototype.uploadDevice();
};
var addDevice = function (devInfo) {
    homeKit.KonkePlatform.prototype.onDevice(devInfo);
};
var removeDevice = function (devInfo) {
    homeKit.KonkePlatform.prototype.removeDevice(devInfo['device_mac']);
};


/*---------------导出方法---------------*/
var deviceControl = {};
deviceControl.DeviceDiscovery = deviceDiscovery;
deviceControl.SearchDeviceInfo = searchDeviceInfo;
deviceControl.SetSwitch = setSwitch;
exports.DeviceControl = deviceControl;

// deviceControl.DeviceDiscovery();
