<?php
//die();////////////////////////////////////remove this
if(isset($_POST['LastInd'])){
    $LastInd=$_POST['LastInd'];
}else{
    $LastInd=0;
}

    require 'vendor/autoload.php';
    use Aws\S3\S3Client;
    $bucket = 'v2c-bucket-v0';
    $s3client = new Aws\S3\S3Client([
        'version' => 'latest',
        'region' => 'eu-west-1',
        'credentials' => [
            'key'    => 'AKIAW5XL6GLMXR5MPZWJ',
            'secret' => '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',
        ],
    ]);
    $ReturnedData = array();
    try {
        $contents = $s3client->listObjects([
            'Bucket' => $bucket,
        ]);
        foreach ($contents['Contents'] as $content){
            try {
                $result = $s3client->getObject([
                    'Bucket' => $bucket,
                    'Key'    => $content['Key'],
                ]);
                $content = $result['Body']->getContents();
                $json = json_decode($content, true);
                if($json['No']>$LastInd){
                    array_push($ReturnedData, $content);
                }
                //$json = json_decode($content, true);
                //echo "<br>JSON data:";
                //print_r($json);
            } catch (S3Exception $e){
                //echo $e->getMessage()."<br>";//Failed to Fetch Data from JSON
                echo "FileFetchFailed";
            }
        }
    } catch (Exception $exception) {
        echo "ListingFailed";
        //echo "Failed to list objects in $bucket with error: " . $exception->getMessage();
    }
    echo(json_encode($ReturnedData));
?>