import { newPlot } from 'plotly.js-dist-min';

const apiUrl = process.env.API_URL;

let dates = [];
let temps = [];
let gases = [];


const plotGraphs = () => {
    // Temperature graph
    newPlot('graph', [{
        x: dates,
        y: temps,
        type: 'scatter',
        mode: 'lines+markers',
        name: 'Temperature'
    }], {
        title: 'Temperature Over Time',
        xaxis: { title: 'Date' },
        yaxis: { title: 'Temperature (°C)' }
    });

    // Gas graph
    newPlot('graph2', [{
        x: dates,
        y: gases,
        type: 'scatter',
        mode: 'lines+markers',
        name: 'Gas Levels'
    }], {
        title: 'Gas Levels Over Time',
        xaxis: { title: 'Date' },
        yaxis: { title: 'Gas Levels (ppm)' }
    });
}


const loadData = async () => {
    try {
        const response = await fetch(apiUrl);
        const data = await response.json();
        console.log(data);

        const table = document.getElementById('data-table-body');
        data.forEach(item => {
            dates.push(item.date);
            temps.push(item.tempReading);
            gases.push(item.gasReading);
            const row = document.createElement('tr');
            const timeCell = document.createElement('td');
            timeCell.textContent = item.date;
            const tempCell = document.createElement('td');
            tempCell.textContent = item.tempReading;
            const gasCell = document.createElement('td');
            gasCell.textContent = item.gasReading;
            row.appendChild(timeCell);
            row.appendChild(tempCell);
            row.appendChild(gasCell);
            table.appendChild(row);

        });
        plotGraphs();
    } catch (error) {
        console.error('Error fetching data:', error);
    }
}

loadData();
