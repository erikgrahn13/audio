set(ONNX_URL https://github.com/microsoft/onnxruntime/releases/download/v1.23.2/onnxruntime-win-x64-1.23.2.zip)

FetchContent_Declare(
  onnx
  URL ${ONNX_URL}
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(onnx)