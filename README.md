# SmartCameraSystem
UE4 Smart Camera System <br />
![ezgif com-gif-maker (1)](https://user-images.githubusercontent.com/10357361/214292268-a8a1c4e3-bec3-472a-bdbf-f523417304ea.gif)


How Event Tick looks like: <br />
Step 1 - Generate cluster: <br />
![image](https://user-images.githubusercontent.com/10357361/214280513-5271e54a-bea4-4698-9f55-80ffc9411420.png)
Step 2 - Once we have generated cluster we need to update new centroids positions: <br />
![image](https://user-images.githubusercontent.com/10357361/214280548-9581b887-3eec-452f-bd2f-b2d3f7c0792c.png)
Step 3 - Once we have generated cluster we need to update new cluster positions- : <br />
![image](https://user-images.githubusercontent.com/10357361/214280591-62a4d980-a222-4394-ad38-1e27126e6d15.png)
Step 4 - Used for visualising KMeans: <br />
![image](https://user-images.githubusercontent.com/10357361/214280630-e917aceb-e229-447c-97ae-3a44a93b9cd4.png)
Step 5 - Get centroids with most clusters: <br />
![image](https://user-images.githubusercontent.com/10357361/214280666-5a71bbc2-7d87-42e5-b40f-d09c294d46d0.png)


Camera positioning:

Get all actors that are facing camera and those who do not
![image](https://user-images.githubusercontent.com/10357361/214281550-d2113ba6-f5b6-4aed-9396-04d70b66677d.png)
![image](https://user-images.githubusercontent.com/10357361/214281603-89f02489-6ead-41d2-946c-b3594ed7c3bc.png)
![image](https://user-images.githubusercontent.com/10357361/214281639-137a5d73-279d-429d-8f43-2eff045210d4.png)

Camera AI Controller <br />
Once we have all actors that are facing camera  and do not. We calculate our look at rotator. If we have more actors that are facing camera.
Then get average rotator in that position otherwise get averga rotator to actors who arer not facing camera.
![image](https://user-images.githubusercontent.com/10357361/214293265-36fa0123-999c-4a1d-baf6-cc76b0c3e6ed.png)
![image](https://user-images.githubusercontent.com/10357361/214293441-2f87f5a9-c7e0-46a4-8c11-b4732baeb201.png)
![image](https://user-images.githubusercontent.com/10357361/214293597-1829e7f2-dc56-4aa3-8acf-145e0c474a69.png)










