const fs = require('fs');
const Path = require('path');

const tf_y_str = fs.readFileSync('./out/tensorflow_y.txt', 'utf-8');
const utensor_y_str = fs.readFileSync('./out/utensor_y.txt', 'utf-8');

function getSubstringAsJson(output, beginStr, endStr) {
    let begin = output.indexOf(beginStr);
    let end = output.indexOf(endStr);
    return JSON.parse(output.substr(begin + beginStr.length, end - begin - beginStr.length));
}

let tf_y = getSubstringAsJson(tf_y_str, 'Begin output', 'End output');
let utensor_y = getSubstringAsJson(utensor_y_str, 'Begin output', 'End output');

if (tf_y.length !== utensor_y.length) {
    console.error('Length of tensorflow_y.txt and utensor_y.txt does not match',
        tf_y.length, 'vs', utensor_y.length);
    process.exit(1);
}

let correct = 0;
let total_fault = 0;

for (let ix = 0; ix < tf_y.length; ix++) {
    let faulty = false;
    for (let jx = 0; jx < tf_y[ix].length; jx++) {
        if (Math.abs(tf_y[ix][jx] - utensor_y[ix][jx]) >= 0.01) {
            faulty = true;
        }
    }

    if (faulty) {
        console.log(`ERR (line: ${ix+1}) - TensorFlow:`, tf_y[ix].map(n=>n.toFixed(5)), ', uTensor:', utensor_y[ix].map(n=>n.toFixed(5)));
        total_fault++;
    }
    else {
        correct++;
    }
}

console.log('Results');
console.log('=======');
console.log('Correct:', correct);
console.log('Faulty: ', total_fault);
