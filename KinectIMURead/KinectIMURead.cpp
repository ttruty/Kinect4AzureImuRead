#pragma comment(lib, "k4a.lib")
#include <k4a/k4a.h>

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <time.h>

#pragma warning(disable:4996)

int main()
{
	int returnCode = 1;
	uint32_t count = k4a_device_get_installed_count();
	const int32_t TIMEOUT_IN_MS = 1000;
	int captureFrameCount;
	k4a_capture_t capture = NULL;
	k4a_imu_sample_t imu_sample;

	
	if (count == 0)
	{
		//printf("No k4a devices attached!\n");
		return 1;
	}

	// Open the first plugged in Kinect device
	k4a_device_t device = NULL;
	if (K4A_FAILED(k4a_device_open(K4A_DEVICE_DEFAULT, &device)))
	{
		//printf("Failed to open k4a device!\n");
		return 1;
	}

	// Get the size of the serial number
	size_t serial_size = 0;
	k4a_device_get_serialnum(device, NULL, &serial_size);

	// Allocate memory for the serial, then acquire it
	char* serial = (char*)(malloc(serial_size));
	k4a_device_get_serialnum(device, serial, &serial_size);
	//printf("Opened device: %s\n", serial);
	free(serial);


	k4a_device_configuration_t config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
	config.color_format = K4A_IMAGE_FORMAT_COLOR_MJPG;
	config.color_resolution = K4A_COLOR_RESOLUTION_2160P;
	config.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
	config.camera_fps = K4A_FRAMES_PER_SECOND_30;

	// Here system_clock is wall clock time from 
	// the system-wide realtime clock 
	// declaring argument of time() 
	time_t t = time(0);   // get time now
	struct tm* now = localtime(&t);

	char buffer[80];
	strftime(buffer, 80, "%Y-%m-%d.", now);
	printf(buffer);
	//strcat("kinect_imu_read", buffer);

	std::ofstream outfile;// declaration of file pointer named outfile
	outfile.open(buffer, std::ios::out); // opens file named "filename" for output
	//outfile << "Hello";//saves "Hello" to the outfile with the insertion operator
	


	// Start the camera with the given configuration
	if (K4A_FAILED(k4a_device_start_cameras(device, &config)))
	{
		printf("Failed to start cameras!\n");
		k4a_device_close(device);
		return 1;
	}

	if (K4A_RESULT_SUCCEEDED != k4a_device_start_imu(device))
	{
		printf("Failed to start imu\n");
		return 1;
	}

	captureFrameCount = 50;
	printf("Capturing %d frames\n", captureFrameCount);

	

	// Camera capture and application specific code would go here
	while (captureFrameCount-- > 0)
	{
		//k4a_image_t image;
		k4a_imu_sample_t imu_sample;

		// Capture a imu sample
		// Capture a imu sample
		switch (k4a_device_get_imu_sample(device, &imu_sample, TIMEOUT_IN_MS))
		{
		case K4A_WAIT_RESULT_SUCCEEDED:
			break;
		case K4A_WAIT_RESULT_TIMEOUT:
			printf("Timed out waiting for a imu sample\n");
			continue;
			break;
		case K4A_WAIT_RESULT_FAILED:
			printf("Failed to read a imu sample\n");
			goto Exit;
		}

			printf("IMU STATS:\ntemp: %.2f \nAcc : x:%.4f y:%.4f z: %.4f\nGyro: x:%.4f y:%.4f z: %.4f\n",
				imu_sample.temperature,
				imu_sample.acc_sample.xyz.x,
				imu_sample.acc_sample.xyz.y,
				imu_sample.acc_sample.xyz.z,
				imu_sample.gyro_sample.xyz.x,
				imu_sample.gyro_sample.xyz.y,
				imu_sample.gyro_sample.xyz.z
			);

			outfile << printf("IMU STATS:\ntemp: %.2f \nAcc : x:%.4f y:%.4f z: %.4f\nGyro: x:%.4f y:%.4f z: %.4f\n",
				imu_sample.temperature,
				imu_sample.acc_sample.xyz.x,
				imu_sample.acc_sample.xyz.y,
				imu_sample.acc_sample.xyz.z,
				imu_sample.gyro_sample.xyz.x,
				imu_sample.gyro_sample.xyz.y,
				imu_sample.gyro_sample.xyz.z
				);
		//// Get a depth frame
		//switch (k4a_device_get_capture(device, &capture, TIMEOUT_IN_MS))
		//{
		//case K4A_WAIT_RESULT_SUCCEEDED:
		//	break;
		//case K4A_WAIT_RESULT_TIMEOUT:
		//	printf("Timed out waiting for a capture\n");
		//	continue;
		//	break;
		//case K4A_WAIT_RESULT_FAILED:
		//	printf("Failed to read a capture\n");
		//	goto Exit;
		//}		

		//printf("Capture");

		//// Probe for a color image
		//image = k4a_capture_get_color_image(capture);
		//if (image)
		//{
		//	printf(" | Color res:%4dx%4d stride:%5d ",
		//		k4a_image_get_height_pixels(image),
		//		k4a_image_get_width_pixels(image),
		//		k4a_image_get_stride_bytes(image));
		//	k4a_image_release(image);
		//}
		//else
		//{
		//	printf(" | Color None                       ");
		//}

		//// probe for a IR16 image
		//image = k4a_capture_get_ir_image(capture);
		//if (image != NULL)
		//{
		//	printf(" | Ir16 res:%4dx%4d stride:%5d ",
		//		k4a_image_get_height_pixels(image),
		//		k4a_image_get_width_pixels(image),
		//		k4a_image_get_stride_bytes(image));
		//	k4a_image_release(image);
		//}
		//else
		//{
		//	printf(" | Ir16 None                       ");
		//}

		//// Probe for a depth16 image
		//image = k4a_capture_get_depth_image(capture);
		//if (image != NULL)
		//{
		//	printf(" | Depth16 res:%4dx%4d stride:%5d\n",
		//		k4a_image_get_height_pixels(image),
		//		k4a_image_get_width_pixels(image),
		//		k4a_image_get_stride_bytes(image));
		//	k4a_image_release(image);
		//}
		//else
		//{
		//	printf(" | Depth16 None\n");
		//}

		// release capture
		//k4a_capture_release(capture);
		fflush(stdout);
	}
	outfile.close();// closes file; always do this when you are done using the file
	returnCode = 0;
Exit:
	if (device != NULL)
	{
		k4a_device_close(device);
	}

	return returnCode;
}

