# Lane Detection Model Specifications

## Model Architecture
- Base: U-Net
- Input size: 256x128x3
- Output size: 256x128x2
- Backbone: ResNet50
- FP16 precision for TensorRT

## Layer Configuration
1. Encoder
   - ResNet18 backbone
   - 5 encoding blocks
   - Feature channels: [64, 128, 256, 512]

2. Decoder
   - 4 upsampling blocks
   - Skip connections from encoder
   - Final 1x1 convolution

## Performance Targets
- Inference time: < 100ms
- Memory footprint: < 2GB
- FPS target: > 15 fps

## Export Formats
1. PyTorch (.pth)
   - Model weights
   - Optimizer state
   - Training metadata

2. ONNX (.onnx)
   - Opset version: 11
   - Dynamic batch size
   - Input name: "input"
   - Output name: "output"

3. TensorRT (.engine)
   - FP16 precision
   - Workspace size: 258MB
   - Max batch size: 1