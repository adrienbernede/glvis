import AppKit
import Foundation

let fname_in = CommandLine.arguments[1]
let fname_out = CommandLine.arguments[2]

let data = NSData.init(contentsOfFile: fname_in)!

let width = 1024
let space = CGColorSpaceCreateDeviceRGB()
let bitmapInfo = CGBitmapInfo(rawValue: CGImageAlphaInfo.premultipliedLast.rawValue)
let dataProvider = CGDataProvider(data: data)!

let cgImage = CGImage.init(
    width: width,
    height: width,
    bitsPerComponent: 8,
    bitsPerPixel: 32,
    bytesPerRow: 4*width,
    space: space,
    bitmapInfo: bitmapInfo,
    provider: dataProvider,
    decode: nil,
    shouldInterpolate: true,
    intent: .defaultIntent
)!

let img = NSImage.init(cgImage: cgImage, size: .zero)
let rep = NSBitmapImageRep(data: img.tiffRepresentation!)!
let png = rep.representation(using: .png, properties: [:])!
NSData.init(data: png).write(toFile: fname_out, atomically: false)
