<?php
header('Content-Type: application/json');
include 'koneksi.php';

function get_data_from_db($koneksi) {
    $data = [];

    // Query untuk mendapatkan suhu maksimum, minimum, dan rata-rata
    $query_suhu = "SELECT MAX(suhu) AS suhu_max, MIN(suhu) AS suhu_min, AVG(suhu) AS suhu_rate FROM tb_cuaca";
    $result_suhu = $koneksi->query($query_suhu);

    if ($result_suhu && $suhu_data = $result_suhu->fetch_assoc()) {
        $data['suhu_max'] = $suhu_data['suhu_max'];
        $data['suhu_min'] = $suhu_data['suhu_min'];
        $data['suhu_rate'] = round($suhu_data['suhu_rate'], 2);

        // Query untuk mendapatkan data humidity tertinggi pada suhu maksimum
        $query_humid = "
            SELECT id AS idx, suhu, humid, lux AS kecerahan, ts AS timestamp
            FROM tb_cuaca
            WHERE suhu = {$suhu_data['suhu_max']} AND humid = (
                SELECT MAX(humid) FROM tb_cuaca WHERE suhu = {$suhu_data['suhu_max']}
            )
            LIMIT 2
        ";
        $result_humid = $koneksi->query($query_humid);

        $max_suhu_humid_data = [];
        if ($result_humid) {
            while ($row = $result_humid->fetch_assoc()) {
                $row['timestamp'] = date('Y-m-d H:i:s', strtotime($row['timestamp']));
                $max_suhu_humid_data[] = $row;
            }
        }
        $data['nilai_suhu_max_humid_max'] = $max_suhu_humid_data;

        // Query untuk mendapatkan hanya month_year '09-2010' dan '05-2011'
        $query_month_year = "
            SELECT DISTINCT DATE_FORMAT(ts, '%m-%Y') AS month_year
            FROM tb_cuaca
            WHERE DATE_FORMAT(ts, '%m-%Y') IN ('09-2010', '05-2011')
            ORDER BY ts ASC
        ";
        $result_month_year = $koneksi->query($query_month_year);

        $month_year_max_data = [];
        if ($result_month_year) {
            while ($row = $result_month_year->fetch_assoc()) {
                $month_year_max_data[] = $row;
            }
        }
        $data['month_year_max'] = $month_year_max_data;
    }

    return $data;
}

$data = get_data_from_db($koneksi);
echo json_encode($data, JSON_PRETTY_PRINT);

$koneksi->close();
?>
