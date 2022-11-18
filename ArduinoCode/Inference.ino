
#include <TensorFlowLite.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"
#include "model.h"
#include <Arduino_LSM9DS1.h>
extern "C"
{
#include "utils.h"
};


// Globals, used for compatibility with Arduino-style sketches.
namespace 
{
#define NumSamples 119

// array to map gesture index to a name
uint32_t Index_u32 = 0;
const char* GESTURES[] = 
{
  "RIGHTLEFT",
  "UPDOWN",
  "IDLE"
};

#define NUM_GESTURES (sizeof(GESTURES) / sizeof(GESTURES[0]))
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;
int inference_count = 0;

float BufferAx[119] = {0.0};
float BufferAy[119] = {0.0};
float BufferAz[119] = {0.0};
constexpr int kTensorArenaSize = 8 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
}  // namespace

// The name of this function is important for Arduino compatibility.
void setup() 
{
 
 Serial.begin(9600);
  while (!Serial);

  // initialize the IMU
  if (!IMU.begin())
  {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // print out the samples rates of the IMUs
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
 
 
  // Set up logging. Google style is to avoid globals or statics because of
  // lifetime uncertainty, but since this has a trivial destructor it's okay.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel( model_g );
  if ( model->version() != TFLITE_SCHEMA_VERSION ) 
  {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // This pulls in all the operation implementations we need.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::AllOpsResolver resolver;

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, kTensorArenaSize, error_reporter );

  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if ( allocate_status != kTfLiteOk )
   {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  // Obtain pointers to the model's input and output tensors.
  input = interpreter->input(0);
  output = interpreter->output(0);




}

void loop() 
{
  // put your main code here, to run repeatedly:
  //inference :
  float Ax, Ay, Az;
  while ( Index_u32 < ( NumSamples ) ) 
  {
      // read the acceleration data
      uint64_t next_tick = micros() + ( 500 / 119 );
      while( !IMU.accelerationAvailable() );
      IMU.readAcceleration( Ax, Ay, Az );
      ///////////////////////////
      BufferAx[Index_u32] = Ax;
      BufferAy[Index_u32] = Ay;
      BufferAz[Index_u32] = Az;
      //////////////////////////
      Index_u32 += 1;
      if( next_tick > micros() )
      {
        delayMicroseconds( next_tick - micros() );
      }  
  }
  Ax = calc_mad( &BufferAx[0U], 119 );
  Ay = calc_mad( &BufferAy[0U], 119 );
  Az = calc_mad( &BufferAz[0U], 119 );
  input->data.f[ 0U ] = ( Ax );
  input->data.f[ 1U ] = ( Ay );
  input->data.f[ 2U ] = ( Az );
    // Run inference, and report any error
  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) 
  {
    TF_LITE_REPORT_ERROR( error_reporter, "Invoke failed" );
    return;
  }
          // Loop through the output tensor values from the model
  for (int i = 0; i < NUM_GESTURES; i++) 
  {
    if( output->data.f[i] > 0.6 )
      {
        Serial.println( GESTURES[i] );
      }  
  }
  Index_u32 = 0;
  delay(250);
}
