<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<title>BMI Calculator with jQuery and Bootstrap</title>
<!-- Bootstrap CSS -->
<link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/
bootstrap.min.css">
<link rel="stylesheet" href="style.css">
<!-- jQuery -->
<script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>
<!-- Bootstrap JS -->
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"
></script>
<script src="script.js" defer></script>
</head>
<body>
<div class="container">
<h1 class="mt-5">BMI Calculator</h1>
<div id="result" class="mt-3"></div>
<form id="bmiForm" class="mt-3">
<div class="form-group">
<label for="name">Name:</label>
<input type="text" id="name" name="name" class="form-control" required>
</div>
<div class="form-group">
<label for="weight">Weight (kg):</label>
<input type="number" id="weight" name="weight" class="form-control" required>
</div>
<div class="form-group">
<label for="height">Height (m):</label>
<input type="number" id="height" name="height" class="form-control" step="0.01"required>
</div>
<button type="submit" class="btn btn-primary">Calculate</button>
</form>
</div>
</body>
</html>


<?php
header('Content-Type: application/json');
if(isset($_POST['name'], $_POST['weight'], $_POST['height'])) {
$name = htmlspecialchars($_POST['name']);
$weight = floatval($_POST['weight']);
$height = floatval($_POST['height']);
if($weight <= 0 || $height <= 0) {
echo json_encode(['success' => false,'message' => 'Invalid input values. Weight and height must be greater than zero.']);
 exit;
 }
 $bmi = $weight / ($height * $height);
if($bmi < 18.5) {
 $interpretation = "Underweight";
 } elseif($bmi < 25) {
 $interpretation = "Normal weight";
 } elseif($bmi < 30) {
 $interpretation = "Overweight";
 } else {
 $interpretation = "Obesity";
}
 $message = "Hello, $name. Your BMI is " . number_format($bmi,2) . " ($interpretation).";
 echo json_encode([
 'success' => true,
 'bmi' => $bmi,
 'message' => $message
 ]);
 exit;
 }
 echo json_encode(['success' => false,'message' => 'Data not received.']);
 exit;
 ?>



 $(document).ready(function(){
    $('#bmiForm').submit(function(e){
    e.preventDefault();
     // Validate inputs using jQuery
    var name = $('#name').val().trim();
     var weight = parseFloat($('#weight').val());
     var height = parseFloat($('#height').val());
     if(name === ""  isNaN(weight)  isNaN(height)  weight <= 0  height <= 0) {
     $('#result').html('<div class="alert alert-warning">Please enter valid values inall fields.</div>');
     return;
     }
     // Send data via AJAX
     $.ajax({
     url: 'calculate.php',
     type: 'POST',
     data: { name: name, weight: weight, height: height },
     dataType: 'json',
success: function(response) {
if(response.success) {
var alertClass = 'alert-info';
if(response.bmi < 18.5) {
alertClass = 'alert-warning';
} else if(response.bmi < 25) {
alertClass = 'alert-success';
} else if(response.bmi < 30) {
alertClass = 'alert-info';
} else {
alertClass = 'alert-danger';
}
$('#result').html('<div class="alert ' + alertClass + '">' + response.
message + '</div>');
} else {
$('#result').html('<div class="alert alert-danger">' + response.message +'</div>');
}
},
error: function() {
$('#result').html('<div class="alert alert-danger">Server error occurred.</div>');
}
});
});
});
