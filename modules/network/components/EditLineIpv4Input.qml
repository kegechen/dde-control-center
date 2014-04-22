import QtQuick 2.1
import Deepin.Widgets 1.0
import "../widgets"

BaseEditLine {
    id: root
    
    rightLoader.sourceComponent: Ipv4Input{
        width: valueWidth
        isError: isValueError()
        
        // TODO add property text for Ipv4Input
        // Binding on text {
        //     when: root.value != undefined
        //     value: root.value
        // }
        
        // TODO fix focus issue
        // onToNext: {
            // var ipAddress = getValue()
            // if(ipAddress){
            //     var netmask = windowView.getDefaultMask(ipAddress)
            //     if(netmask){
             //         netmaskLine.rightLoader.item.setValue(netmask)
            //     }
            // }
            // netmaskLine.rightLoader.item.getFocus()
        // }
        onIsFocusChanged: {
            if(!isFocus){
                root.value = getValue()
                setKey()
            }
        }
        
        // Binding {
        // }
        
        // make onVisibleChanged() wrapped by Connections to ensure
        // code excuted after EditLineIpv4Input.onVisibleChanged()
        Connections {
            target: root
            onVisibleChanged: {
                if (root.visible) {
                    if (root.value) {
                        setValue(root.value)
                    }
                }
                // TODO test
                print("EditLineIpv4Input.onVisibleChanged", visible ? "(show)" : "(hide)", section, key, getValue())
            }
        }
        
        // Component.onCompleted() is still need for that
        // onVisibleChanged() will not be called when widget loaded
        Component.onCompleted: {
            if (root.visible) {
                root.value = getKey() // getKey() is need here
                if (root.value) {
                    setValue(root.value)
                }
            }
        }
    }
    
    function saveKey() {
        print("ipv4 save key", section, key, value) // TODO test
        value = root.rightLoader.item.getValue()
        setKey()
    }
}
