import { newPlot } from 'plotly.js-dist-min';

const apiUrl = process.env.API_URL;

async function loadData() {
    try {
        const response = await fetch(apiUrl);
        const data = await response.json();

        const table = document.getElementById('data-table-body');
        data.forEach(item => {
            const row = document.createElement('tr');
            const cell = document.createElement('td');
            cell.textContent = item;
            row.appendChild(cell);
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

