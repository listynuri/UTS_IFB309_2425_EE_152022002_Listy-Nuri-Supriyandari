fetch('data.php')
    .then(response => {
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        return response.json();
    })
    .then(data => {
        // Menampilkan data suhu maksimum, minimum, dan rata-rata
        document.getElementById('suhuMax').textContent = `${data.suhumax}°C`;
        document.getElementById('suhuMin').textContent = `${data.suhumin}°C`;
        document.getElementById('suhuRata').textContent = `${data.suhurata}°C`;

        // Menampilkan data suhu dan kelembaban maksimum dengan animasi
        const dataDisplay = document.getElementById('dataDisplay');
        dataDisplay.innerHTML = '';
        data.nilai_suhu_max_humid_max.forEach((item, index) => {
            const dataItem = document.createElement('div');
            dataItem.className = 'data-item';
            dataItem.innerHTML = `
                <p><strong>Index:</strong> ${item.idx}</p>
                <p><strong><i class="fas fa-thermometer-half"></i> Suhu Max:</strong> ${item.suhumax}°C</p>
                <p><strong><i class="fas fa-tint"></i> Kelembaban:</strong> ${item.humid}%</p>
                <p><strong><i class="fas fa-sun"></i> Kecerahan:</strong> ${item.kecerahan}</p>
                <p><strong><i class="fas fa-calendar-alt"></i> Timestamp:</strong> ${item.timestamp}</p>
            `;
            setTimeout(() => {
                dataDisplay.appendChild(dataItem);
                dataItem.style.opacity = '1';
                dataItem.style.transform = 'translateY(0)';
            }, index * 200); // Penundaan 200ms untuk setiap item
        });

        // Menampilkan data bulan dan tahun maksimum dengan animasi
        const monthYearMax = document.getElementById('monthYearMax');
        monthYearMax.innerHTML = '';
        data.month_year_max.forEach((item, index) => {
            const monthYearItem = document.createElement('p');
            monthYearItem.innerHTML = `<i class="fas fa-calendar-alt"></i> ${item.month_year}`;
            monthYearItem.style.opacity = '0';
            monthYearItem.style.transform = 'translateY(10px)';
            monthYearMax.appendChild(monthYearItem);

            setTimeout(() => {
                monthYearItem.style.opacity = '1';
                monthYearItem.style.transform = 'translateY(0)';
            }, index * 200); // Penundaan 200ms untuk setiap item
        });
    })
    .catch(error => {
        console.error('Error fetching data:', error);
    });
