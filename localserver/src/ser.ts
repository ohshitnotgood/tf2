import { SerialPort } from "serialport";


let transmittableValue = () => {
    return `a=${aValue};b=${bValue}`
}

let aValue = "0,0"
let bValue = "120,0"


export function setAValue(value: string) {
    aValue = value
    console.log(transmittableValue())
}

export function setBValue(value: string) {
    bValue = value
    console.log(transmittableValue())
}
