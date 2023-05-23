make
:: qemu-system-arm -drive format=raw,file=boot.iso -d cpu_reset -monitor stdio -device sb16 -audiodev coreaudio,id=coreaudio,out.frequency=48000,out.channels=2,out.format=s32
