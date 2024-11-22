import { newPlot } from 'plotly.js-dist-min';

const apiUrl = process.env.API_URL;

async function loadData() {
    try {
        const response = await fetch(apiUrl);
        const data = await response.json();
        console.log(data);

        const table = document.getElementById('data-table-body');
        data.forEach(item => {
            const row = document.createElement('tr');
            const timeCell = document.createElement('td');
            timeCell.textContent = item.time;
            const tempCell = document.createElement('td');
            tempCell.textContent = item.temp;
            row.append(timeCell);
            row.appendChild(tempCell);
            table.appendChild(row);
        });
    } catch (error) {
        console.error('Error fetching data:', error);
    }
}

newPlot('graph', [{
    x: [1, 2, 3, 4, 5],
    y: [1, 2, 4, 8, 16],
    type: 'scatter'
}]);


loadData();

