<?php 
  $PageTitle = "Login";
	session_start();
	$username = "";
	$errors = array();

	header('Access-Control-Allow-Origin: *');
	header('Access-Control-Allow-Methods: GET, POST');
	header("Access-Control-Allow-Headers: X-Requested-With");


	//unset($_SESSION['username']);
	if (isset($_GET['logout'])) {
    session_destroy();
    unset($_SESSION['username']);
    header("location: login.php");die();
	}

	if (isset($_SESSION['username'])) {
    $_SESSION['msg'] = "You must log in first";header('location: index.php');die();
	}

	if (isset($_POST['login_user'])) {
	  $username = filter_var($_POST['Username'],FILTER_SANITIZE_STRING);
	  $password = filter_var($_POST['Password'],FILTER_SANITIZE_STRING);
	  if (empty($username)) {
	  	array_push($errors, "Username is required");
	  }
	  if (empty($password)) {
	  	array_push($errors, "Password is required");
	  }
	  if (count($errors) == 0) {
	  	//$password = md5($password);

	  	$data = ['id'=>'12314','username'=>'ITI','password'=>'123'];


	    if($data == null){
	      array_push($errors, "Wrong username/password combination");
	    }else{


	    	if($username == $data['username'] && $password == $data['password']){
	    		$_SESSION['username'] = $data['username'];
	    		header('location: index.php');die();
	    	}else{
	    		array_push($errors, "Wrong username/password combination");
	    	}

	    	
	    }
	  }
	}
?>
<!DOCTYPE html>
<html>
<head>
	<link rel="preconnect" href="https://fonts.googleapis.com">
	<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
	<link href="https://fonts.googleapis.com/css2?family=Poppins:wght@300;400;500;600;700;800;900&display=swap" rel="stylesheet">

	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<meta name="robots" content="index, follow">
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta property="og:type" content="website" />
	<meta property="twitter:card" content="summary_large_image" />
	<meta name="msapplication-TileColor" content="#ffffff">
	<meta name="theme-color" content="#ffffff">
	<link rel="stylesheet" type="text/css" href="css/style.css">
</head>
<body class="BrightNav TransparentNav">
	<div class="LoginHero">
		<div class="Overlay"></div>
		<div class="BackgroundContainer">
			<div class="Overlay"></div>


			<div class="FormContainer">

				<img class="BrandLoginIcon Bright" src="img/ITINexusWhite.svg">


				<form class="MT10" autocomplete="off" method="post" action="login.php">
					<div class="InputCont">
						<input type="text" name="Username" id="UsernameInput" placeholder="Username" required>
						<img alt="User Icon" src="img/User.svg">
					</div>


					<div class="InputCont">
						<input type="password" name="Password" id="PasswordInput" placeholder="Password" required>
						<img alt="Password Icon" src="img/lock.svg">
					</div>



					<!--<div class="Separator"></div>-->

					<?php  if(count($errors) > 0):?>
						<div class="error">
					  	<?php foreach ($errors as $error) : ?>
					  	  <p><?php echo $error ?></p>
					  	<?php endforeach ?>
					  </div>
					<?php endif?>






					<div align="center" class="FullWidth MT40">
						<input type="submit" name="login_user" value="Sign In" class="BrandBtn">
					</div>

				</form>


				
				
				


			</div>

			



			


		</div>
		





	</div>















</body>
</html>