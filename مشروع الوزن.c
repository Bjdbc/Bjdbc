هيكل المشروع
bmi_project/
│
├── config/
│   └── database.php       # إعداد الاتصال بقاعدة البيانات
│
├── app/
│   ├── models/
│   │   └── BmiModel.php   # نموذج البيانات: يتعامل مع قاعدة البيانات
│   │
│   ├── controllers/
│   │   └── BmiController.php # المتحكم: يحتوي منطق حساب BMI
│   │
│   └── views/
│       ├── bmi_form.php   # نموذج HTML لإدخال الوزن والطول
│       └── bmi_result.php # صفحة عرض نتيجة حساب BMI
│
├── public/
│   └── index.php          # نقطة الدخول للتطبيق (تشغيل المشروع)
│
└── .htaccess              # يمكن استخدامه لإخفاء مسارات URL إن لزم الأمر


---

1. config/database.php

<?php
// إعدادات الاتصال بقاعدة البيانات
$host = 'localhost';
$dbname = 'bmi_project';      // اسم قاعدة البيانات
$username = 'root';           // اسم المستخدم
$password = '';               // كلمة المرور

// إنشاء اتصال باستخدام mysqli
$mysqli = new mysqli($host, $username, $password, $dbname);

// التحقق من نجاح الاتصال
if ($mysqli->connect_error) {
    die("Connection failed: " . $mysqli->connect_error); // عرض الخطأ وإيقاف البرنامج إذا فشل الاتصال
}
?>


---

2. app/models/BmiModel.php

<?php
// نموذج البيانات (Model): يتعامل مباشرة مع قاعدة البيانات

class BmiModel {
    private $db;

    public function __construct($database) {
        $this->db = $database; // تخزين الاتصال بقاعدة البيانات
    }

    // حفظ سجل BMI في جدول bmi_records
    public function saveBmiRecord($userId, $name, $weight, $height, $bmi, $status) {
        $stmt = $this->db->prepare("
            INSERT INTO bmi_records (user_id, name, weight, height, bmi, status)
            VALUES (?, ?, ?, ?, ?, ?)
        ");
        $stmt->bind_param("issdss", $userId, $name, $weight, $height, $bmi, $status);
        return $stmt->execute(); // تنفيذ العملية وإرجاع النتيجة
    }

    // استرجاع كل سجلات BMI لمستخدم معين
    public function getBmiHistory($userId) {
        $stmt = $this->db->prepare("
            SELECT * FROM bmi_records
            WHERE user_id = ?
            ORDER BY timestamp DESC
        ");
        $stmt->bind_param("i", $userId);
        $stmt->execute();
        $result = $stmt->get_result(); // جلب النتائج
        return $result->fetch_all(MYSQLI_ASSOC); // تحويل النتائج إلى مصفوفة
    }
}
?>


---

3. app/controllers/BmiController.php

<?php
// المتحكم (Controller): يعالج بيانات النموذج ويحسب BMI

class BmiController {
    private $model;

    public function __construct($model) {
        $this->model = $model; // ربط المتحكم بالنموذج (Model)
    }

    // دالة لحساب BMI بناءً على بيانات المستخدم
    public function calculateBmi($name, $weight, $height, $userId) {
        // الصيغة الرياضية لحساب BMI
        $bmi = $weight / (($height / 100) ** 2);

        // تحديد حالة المستخدم بناءً على BMI
        if ($bmi < 18.5) {
            $status = 'Underweight';  // نقص وزن
        } elseif ($bmi < 25) {
            $status = 'Normal weight'; // وزن طبيعي
        } elseif ($bmi < 30) {
            $status = 'Overweight';    // زيادة وزن
        } else {
            $status = 'Obese';         // سمنة
        }

        // حفظ السجل في قاعدة البيانات
        $this->model->saveBmiRecord($userId, $name, $weight, $height, $bmi, $status);

        // إرجاع النتائج إلى صفحة العرض
        return ['name' => $name, 'bmi' => $bmi, 'status' => $status];
    }
}
?>


---

4. app/views/bmi_form.php

<!-- نموذج المستخدم لإدخال البيانات -->
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Calculate BMI</title>
    <!-- ربط Bootstrap لتحسين المظهر -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@4.5.2/dist/css/bootstrap.min.css" rel="stylesheet">
</head>
<body>
<div class="container mt-5">
    <h2>Calculate Your BMI</h2>
    <!-- نموذج لإدخال الاسم، الوزن، الطول -->
    <form method="POST" action="">
        <div class="form-group">
            <label>Name:</label>
            <input type="text" name="name" class="form-control" required>
        </div>
        <div class="form-group">
            <label>Weight (kg):</label>
            <input type="number" name="weight" class="form-control" required>
        </div>
        <div class="form-group">
            <label>Height (cm):</label>
            <input type="number" name="height" class="form-control" required>
        </div>
        <button type="submit" class="btn btn-primary">Calculate</button>
    </form>
</div>
</body>
</html>


---

5. app/views/bmi_result.php

<!-- عرض نتيجة حساب BMI -->
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>BMI Result</title>
    <!-- ربط Bootstrap لتحسين التصميم -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@4.5.2/dist/css/bootstrap.min.css" rel="stylesheet">
</head>
<body>
<div class="container mt-5">
    <h2>BMI Result</h2>
    <!-- عرض النتائج المحسوبة -->
    <p><strong>Name:</strong> <?= htmlspecialchars($result['name']) ?></p>
    <p><strong>BMI:</strong> <?= number_format($result['bmi'], 2) ?></p>
    <p><strong>Status:</strong> <?= $result['status'] ?></p>
    <a href="index.php" class="btn btn-secondary">Back</a>
</div>
</body>
</html>


---

6. public/index.php

<?php
// ملف التشغيل الرئيسي للتطبيق

// استدعاء ملفات الإعدادات والمكونات الأساسية
require '../config/database.php';
require '../app/models/BmiModel.php';
require '../app/controllers/BmiController.php';

// إنشاء كائنات من النموذج والمتحكم
$model = new BmiModel($mysqli);
$controller = new BmiController($model);

// بدء الجلسة (ضروري لتسجيل الدخول لاحقاً)
session_start();

// تمثيل مؤقت للمستخدم (سنستخدم ID 1 مؤقتاً)
$userId = 1;

// التحقق إذا تم إرسال النموذج
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // قراءة البيانات من المستخدم
    $name = $_POST['name'];
    $weight = $_POST['weight'];
    $height = $_POST['height'];

    // حساب BMI عن طريق المتحكم
    $result = $controller->calculateBmi($name, $weight, $height, $userId);

    // عرض النتيجة في الصفحة المخصصة
    require '../app/views/bmi_result.php';
} else {
    // في حالة عدم إرسال البيانات، نعرض النموذج
    require '../app/views/bmi_form.php';
}
?>


---

7. قاعدة البيانات (تنفيذ SQL في phpMyAdmin)

-- جدول المستخدمين
CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL,
    role ENUM('user', 'admin') NOT NULL DEFAULT 'user'
);

-- جدول سجلات BMI
CREATE TABLE bmi_records (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL,
    name VARCHAR(100) NOT NULL,
    weight FLOAT NOT NULL,
    height FLOAT NOT NULL,
    bmi FLOAT NOT NULL,
    status VARCHAR(50) NOT NULL,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
);
--

