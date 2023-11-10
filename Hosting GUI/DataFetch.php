<?php
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
    try {
        $contents = $s3client->listObjects([
            'Bucket' => $bucket,
        ]);        
        foreach ($contents['Contents'] as $content) {
            try {
                $result = $s3client->getObject([
                    'Bucket' => $bucket,
                    'Key'    => $content['Key'],
                ]);
                $content = $result['Body']->getContents();
                $json = json_decode($content, true);
                echo "<br>JSON data:";
                print_r($json);
            } catch (S3Exception $e){
                //Failed to Fetch Data from JSON
                echo $e->getMessage()."<br>";
            }
        }
    } catch (Exception $exception) {
        echo "Failed to list objects in $bucket with error: " . $exception->getMessage();
        exit("Please fix error with listing objects before continuing.");
    }














    /*
    $keyPrefix = 'your-prefix';

    $objects = $s3client->getIterator('ListObjectsV2', [
        'Bucket' => $bucket,
        'Prefix' => $keyPrefix,
    ]);

    foreach ($objects as $object) {
        if (substr($object['Key'], -5) === '.json') {
            $result = $s3client->getObject([
                'Bucket' => $bucket,
                'Key' => $object['Key'],
            ]);

            $json = json_decode($result['Body'], true);

            echo "Object: " . $object['Key'] . "<br>";
            echo "JSON content:<br>";
            echo "<pre>";
            print_r($json);
            echo "</pre>";
        }
    }
*/












/*


    $s3 = new Aws\S3\S3Client([
        'version' => 'latest',
        'region' => 'eu-west-1',
        'credentials' => [
            'key'    => 'AKIAW5XL6GLMXR5MPZWJ',
            'secret' => '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',
        ],
    ]);


    

    try {
        $result = $client->listObjects([
            'Bucket' => $bucketName,
        ]);
        foreach ($result['Contents'] as $object) {
            echo $object['Key'] . "<br>";
        }
    } catch (S3Exception $e) {
        echo $e->getMessage() . "<br>";
    }



















/*
    $s3 = new Aws\S3\S3Client([
        'region' => 'eu-west-1'  // Since version 3.277.10 of the SDK,
    ]);
    // Create an SDK class used to share configuration across clients.
    $sdk = new Aws\Sdk($sharedConfig);
    // Create an Amazon S3 client using the shared configuration data.
    $client = $sdk->createS3();





    $bucket = 'testbucket';
    $keyname = 'test.txt';

    // Instantiate the client.
    $s3 = S3Client::factory();

    try {
        // Upload data.
        $result = $s3->putObject(array(
            'Bucket' => $bucket,
            'Key'    => $keyname,
            'Body'   => 'Hello, world!',
            'ACL'    => 'public-read'
        ));

        // Print the URL to the object.
        echo $result['ObjectURL'] . "\n";
    } catch (S3Exception $e) {
        echo $e->getMessage() . "\n";
    }


    $movie = $service->getItemByKey($tableName, $key);
    echo "\nThe movie {$movie['Item']['title']['S']} was released in {$movie['Item']['year']['N']}.\n";

    public function getItemByKey(string $tableName, array $key){
        return $this->dynamoDbClient->getItem([
            'Key' => $key['Item'],
            'TableName' => $tableName,
        ]);
    }
*/

?>