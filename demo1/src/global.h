//// BGR vs HSV
#define HSV 1

#define BLOB_HSV_HUE    156
#define BLOB_HSV_SAT    42
#define BLOB_HSV_VAL    222
#define EDGE_HSV_HUE    159
#define EDGE_HSV_SAT    65
#define EDGE_HSV_VAL    200
#define CORE_HSV_HUE    153
#define CORE_HSV_SAT    11
#define CORE_HSV_VAL    252

#define HSV_HUE_LOW     140
#define HSV_SAT_LOW     30
#define HSV_VAL_LOW     190
#define HSV_HUE_HIGH    195
#define HSV_SAT_HIGH    163
#define HSV_VAL_HIGH    240

#define BLOB_BGR_BLUE   174
#define BLOB_BGR_GREEN  162
#define BLOB_BGR_RED    223
#define EDGE_BGR_BLUE   180
#define EDGE_BGR_GREEN  180
#define EDGE_BGR_RED    255
#define CORE_BGR_BLUE   255
#define CORE_BGR_GREEN  255
#define CORE_BGR_RED    255

#define BGR_BLUE_LOW      0
#define BGR_GREEN_LOW     0
#define BGR_RED_LOW     180
#define BGR_BLUE_HIGH   182
#define BGR_GREEN_HIGH  182
#define BGR_RED_HIGH    255

//// Camera
#define CAMERA_NUMBER      2 // list available cameras with 'v4l2-ctl --list-devices'
#define MIN_ROW            0 
#define MIN_COL            0
#define MAX_ROW          480
#define MAX_COL          640
#define FPS               30
#define BUFFERSIZE         1
#define FRAME_FIFO_SIZE   10     // How many frames do we want in our buffer?


//// Blob Detection
#define MAX_MINESWEEP_OFFSET 10 // will check (x +/- offset, y +/- offset) for neighbor pixels
#define NEIGHBOR_COUNT        8 // # of neighbor pixels required to constitute as a "core" pixel (any less will be edge)


//// Blob Selection
// Filters
#define FILTER_BLOB_HSV_HUE   1
#define FILTER_BLOB_HSV_SAT   0
#define FILTER_BLOB_HSV_VAL   1
#define FILTER_EDGE_HSV_HUE   1
#define FILTER_EDGE_HSV_SAT   0
#define FILTER_EDGE_HSV_VAL   1
#define FILTER_CORE_HSV_HUE   1
#define FILTER_CORE_HSV_SAT   0
#define FILTER_CORE_HSV_VAL   1

#define FILTER_BLOB_BGR_BLUE  0
#define FILTER_BLOB_BGR_GREEN 0
#define FILTER_BLOB_BGR_RED   1
#define FILTER_EDGE_BGR_BLUE  0
#define FILTER_EDGE_BGR_GREEN 0
#define FILTER_EDGE_BGR_RED   1
#define FILTER_CORE_BGR_BLUE  1
#define FILTER_CORE_BGR_GREEN 1
#define FILTER_CORE_BGR_RED   1

#define FILTER_SIZE           0

// Weights / Importance
#ifdef HSV
#define BLOB_BLUE_WEIGHT      2 // HSV not BGR (TODO: make this less confusing)
#define BLOB_GREEN_WEIGHT     1 // HSV not BGR (TODO: make this less confusing)
#define BLOB_RED_WEIGHT       1 // HSV not BGR (TODO: make this less confusing)
#define EDGE_BLUE_WEIGHT      2 // HSV not BGR (TODO: make this less confusing)
#define EDGE_GREEN_WEIGHT     1 // HSV not BGR (TODO: make this less confusing)
#define EDGE_RED_WEIGHT       1 // HSV not BGR (TODO: make this less confusing)
#define CORE_BLUE_WEIGHT      1 // HSV not BGR (TODO: make this less confusing)
#define CORE_GREEN_WEIGHT     1 // HSV not BGR (TODO: make this less confusing)
#define CORE_RED_WEIGHT       1 // HSV not BGR (TODO: make this less confusing)
#define SCORE_CUTOFF       1000
#else
#define BLOB_BLUE_WEIGHT      1 
#define BLOB_GREEN_WEIGHT     1 
#define BLOB_RED_WEIGHT       2 
#define EDGE_BLUE_WEIGHT      1 
#define EDGE_GREEN_WEIGHT     1 
#define EDGE_RED_WEIGHT       4 
#define CORE_BLUE_WEIGHT      1 
#define CORE_GREEN_WEIGHT     1 
#define CORE_RED_WEIGHT       1 
#define SCORE_CUTOFF       1000
#endif

#define SIZE_IDEAL            4 // #Pixels in blob
#define SIZE_WEIGHT           0 // 


//// Pong
#define BALL_RED     150
#define BALL_GREEN     0
#define BALL_BLUE     50
#define PADDLE_RED   100 
#define PADDLE_GREEN   0 
#define PADDLE_BLUE    0 
#define BALL_VELOCITY 10


//// Debugging
#define DEBUG            1     // Choose whether to display certain messages 


