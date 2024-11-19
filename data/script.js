const canvas = document.getElementById("linePlot");
const ctx = canvas.getContext("2d");

const maxPoints = 100; // Maximum number of points displayed on the graph
let dataPoints = [];  // Store incoming data points

// Function to add new data point and keep the array at maxPoints size
function addDataPoint(value) {
    if (dataPoints.length >= maxPoints) {
        dataPoints.shift(); // Remove the oldest point if maxPoints is reached
    }
    dataPoints.push(value);
}
function drawLinePlot() {
    ctx.clearRect(0, 0, canvas.width, canvas.height); // Clear canvas for redrawing

    const xPadding = 40;
    const yPadding = 40;
    const maxDataValue = Math.max(...dataPoints, 1);
    const xInterval = (canvas.width - 2 * xPadding) / (maxPoints - 1);
    const yScale = (canvas.height - 2 * yPadding) / maxDataValue;

    // Draw grid
    ctx.strokeStyle = "#e0e0e0";
    ctx.lineWidth = 1;

    // Horizontal grid lines and labels

    var objectiveValue = 20;
    const objectiveY = canvas.height - yPadding - ((objectiveValue / maxDataValue) * (canvas.height - 2 * yPadding)); // Calculate Y position for objectiveValue

    const numGridLines = 10;
    for (let i = 0; i <= numGridLines; i++) {
        const y = canvas.height - yPadding - (i * (canvas.height - 2 * yPadding)) / numGridLines;
        const label = Math.round((maxDataValue / numGridLines) * i);

        // Draw grid line
        ctx.beginPath();
        ctx.moveTo(xPadding, y);
        ctx.lineTo(canvas.width - xPadding, y);
        ctx.stroke();

        // Draw label
        ctx.fillStyle = "black";
        ctx.fillText(label, xPadding - 40, y + 5); // Adjust position as needed
    }

    ctx.beginPath();
    ctx.moveTo(xPadding, objectiveY);
    ctx.lineTo(canvas.width - xPadding, objectiveY);
    ctx.strokeStyle = "green"; // Choose color for the objective line
    ctx.lineWidth = 2;       // Make it slightly thicker for emphasis
    ctx.stroke();
    ctx.fillStyle = "green";
    ctx.fillText(objectiveValue, xPadding - 40, objectiveY);



    ctx.strokeStyle = "lightgray";
    ctx.lineWidth = 1;
    // Vertical grid lines
    for (let i = 0; i < maxPoints; i++) {
        const x = xPadding + i * xInterval;

        ctx.beginPath();
        ctx.moveTo(x, yPadding);
        ctx.lineTo(x, canvas.height - yPadding);
        ctx.stroke();
    }

    // Draw axes
    ctx.strokeStyle = "black";
    ctx.lineWidth = 2;
    ctx.beginPath();
    ctx.moveTo(xPadding, yPadding);
    ctx.lineTo(xPadding, canvas.height - yPadding);
    ctx.lineTo(canvas.width - xPadding, canvas.height - yPadding);
    ctx.stroke();

    // X and Y axis labels
    ctx.fillStyle = "black";
    ctx.font = "20px Arial";

    // Draw data line
    ctx.strokeStyle = "darkcyan";
    ctx.lineWidth = 2;
    ctx.beginPath();

    dataPoints.forEach((point, index) => {
        const x = xPadding + index * xInterval;
        const y = canvas.height - yPadding - point * yScale;

        if (index === 0) {
            ctx.moveTo(x, y);
        } else {
            ctx.lineTo(x, y);
        }
    });

    ctx.stroke(); // Draw the line connecting points

    // Draw data points as dots

    var dpCount = dataPoints.length;
    var dpi = 0;
    dataPoints.forEach((point, index) => {
        const x = xPadding + index * xInterval;
        const y = canvas.height - yPadding - point * yScale;

        ctx.beginPath();
        ctx.arc(x, y, 3, 0, Math.PI * 2);
        ctx.fillStyle = "blue";
        ctx.fill();
        
        if(dpi == dpCount - 1)
        {
            ctx.fillStyle = "black";
            ctx.font = "20px Arial";
            ctx.fillText(point, x - 5, y - 10);
        }
        
        dpi++;
    });
    ctx.stroke(); // Draw the connecting line for the plot

}
// Function to fetch data from the server using AJAX
function fetchData() {
    const xhr = new XMLHttpRequest();
    xhr.open("GET", "/data", true); // Update URL to your server endpoint

    xhr.onload = function () {
        if (xhr.status === 200) {

            var n = Number(xhr.responseText);
            // console.log(n);
            // console.log(typeof n);
            addDataPoint(n / 10);
            drawLinePlot();


            document.getElementById("lastValue").innerText = "Distancia actual: "+n /10 + " cm";
        }
    };

    xhr.send();
}

// Update data every second (adjust as needed)
setInterval(fetchData, 50);

document.getElementById("objectiveValue").innerText = "Objetivo: 20 cm";

function sendK()
{
    var newk = document.getElementById("K").value;

    const xhr = new XMLHttpRequest();
    xhr.open("GET", '/k/'+newk, true);
    xhr.onload = function () {
        if (xhr.status === 200) {
            console.log("ok");
        }
    }
    xhr.send();
}

function sendObj()
{
    var newo = document.getElementById("Obj").value;

    const xhr = new XMLHttpRequest();
    xhr.open("GET", '/obj/'+newo, true);
    xhr.onload = function () {
        if (xhr.status === 200) {
            console.log("ok");
        }
    }
    xhr.send();
}