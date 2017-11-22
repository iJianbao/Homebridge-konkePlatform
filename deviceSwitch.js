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
    // addDevice(presentDevListAry[0]);
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
    var addAry = array_difference(presentDevlist, deviceListAry);
    var deleteAry = array_difference(deviceListAry, presentDevlist);
    console.log('新增设备=', addAry);
    console.log('删除设备=', deleteAry);
    for (var index in addAry) {
        var obj = addAry[index];
        addDevice(obj);
    }
    for (index in deleteAry) {
        obj = deleteAry[index];
        removeDevice(obj);
    }
    deviceListAry = presentDevlist;
};

//去数组之间的差
function array_difference(a, b) { // 差集 a - b
    //clone = a
    var clone = a.slice(0);
    for(var i = 0; i < b.length; i ++) {
        var temp = b[i]['device_mac'];
        for(var j = 0; j < clone.length; j ++) {
            if(temp === clone[j]['device_mac']) {
                //remove clone[j]
                clone.splice(j,1);
            }
        }
    }
    return array_remove_repeat(clone);
}

//去除重复
function array_remove_repeat(a) { // 去重
    var r = [];
    for(var i = 0; i < a.length; i ++) {
        var flag = true;
        var temp = a[i];
        for(var j = 0; j < r.length; j ++) {
            if(temp['device_mac'] === r[j]['device_mac']) {
                flag = false;
                break;
            }
        }
        if(flag) {
            r.push(temp);
        }
    }
    return r;
}

//4：设置开关
var setSwitch = function (devMac, value, callBack) {
    var openClose = value ? 'open' : 'close';
    console.log('开关setSwitch', devMac, 'value', openClose);
    var result = libm.OperateDeviceRelay(devMac, openClose);
    console.log('开关setSwitch-result=', result);
    callBack (result);
};

/*---------------homekit方法------------*/
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
