#include "camera.h"

camera_config_t config;

void cameraSetup(framesize_t frameSize)
{
	pinMode(FLASH_LED, OUTPUT);
	// configure LED PWM functionalitites (ledChannel, freq, resolution);
	ledcSetup(15, 5000, 8);
	ledcAttachPin(FLASH_LED, 15);
	ledcWrite(15, 0); // Flash led OFF

	config.ledc_channel = LEDC_CHANNEL_0;
	config.ledc_timer = LEDC_TIMER_0;
	config.pin_d0 = Y2_GPIO_NUM;
	config.pin_d1 = Y3_GPIO_NUM;
	config.pin_d2 = Y4_GPIO_NUM;
	config.pin_d3 = Y5_GPIO_NUM;
	config.pin_d4 = Y6_GPIO_NUM;
	config.pin_d5 = Y7_GPIO_NUM;
	config.pin_d6 = Y8_GPIO_NUM;
	config.pin_d7 = Y9_GPIO_NUM;
	config.pin_xclk = XCLK_GPIO_NUM;
	config.pin_pclk = PCLK_GPIO_NUM;
	config.pin_vsync = VSYNC_GPIO_NUM;
	config.pin_href = HREF_GPIO_NUM;
	config.pin_sscb_sda = SIOD_GPIO_NUM;
	config.pin_sscb_scl = SIOC_GPIO_NUM;
	config.pin_pwdn = PWDN_GPIO_NUM;
	config.pin_reset = RESET_GPIO_NUM;
	config.xclk_freq_hz = 20000000;
	config.pixel_format = PIXFORMAT_JPEG;
	config.fb_count = 1;
	//init with high specs to pre-allocate larger buffers
	if (psramFound() && frameSize == FRAMESIZE_UXGA)
	{
		config.frame_size = FRAMESIZE_UXGA;
		config.jpeg_quality = 10;
	}
	else
	{
		config.frame_size = frameSize;
		config.jpeg_quality = 15;
	}

#if defined(CAMERA_MODEL_ESP_EYE)
	pinMode(13, INPUT_PULLUP);
	pinMode(14, INPUT_PULLUP);
#endif

	// camera init
	esp_err_t err = esp_camera_init(&config);
	if (err != ESP_OK)
	{
		Serial.printf("Camera init failed with error 0x%x", err);
		return;
	}

	sensor_t *s = esp_camera_sensor_get();
	//initial sensors are flipped vertically and colors are a bit saturated
	if (s->id.PID == OV3660_PID)
	{
		s->set_vflip(s, 1);				//flip it back
		s->set_brightness(s, 1);	//up the blightness just a bit
		s->set_saturation(s, -2); //lower the saturation
	}

#if defined(CAMERA_MODEL_M5STACK_WIDE)
	s->set_vflip(s, 1);
	s->set_hmirror(s, 1);
#endif
}

/* void takePhoto()
{
	// Take Picture with Camera
	ledcWrite(15, 255); // Flash led ON
	camera_fb_t *fb = esp_camera_fb_get();
	if (!fb)
	{
		Serial.println("Camera capture failed");
	}
	ledcWrite(15, 0); // Flash led OFF

	// Save picture to memory
	String myFile = savePhoto(fb);
	if (myFile != "")
	{
		if (!myBot.sendPhotoByFile(msg.sender.id, myFile, filesystem))
			Serial.println("Photo send failed");
		//If you don't need to keep image in memory, delete it
		if (KEEP_IMAGE == false)
		{
			//filesystem.remove("/" + myFile);
		}
	}
	esp_camera_fb_return(fb);
}*/

void camera_Init()
{
	// Init the camera
	cameraSetup(FRAMESIZE_UXGA); //QVGA|CIF|VGA|SVGA|XGA|SXGA
}