<?php
header('Content-Type: application/json');

$data = [
    "suhumax" => 36,
    "suhumin" => 21,
    "suhurata" => 28.35,
    "nilai_suhu_max_humid_max" => [
        [
            "idx" => 101,
            "suhumax" => 36,
            "humid" => 36,
            "kecerahan" => 25,
            "timestamp" => "2010-09-18 07:23:48"
        ],
        [
            "idx" => 226,
            "suhumax" => 36,
            "humid" => 36,
            "kecerahan" => 27,
            "timestamp" => "2011-05-02 12:29:34"
        ]
    ],
    "month_year_max" => [
        ["month_year" => "9-2010"],
        ["month_year" => "5-2011"]
    ]
];

// Mengonversi data PHP ke format JSON
echo json_encode($data);
?>
