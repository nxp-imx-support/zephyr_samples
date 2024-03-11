# i.MX Zephyr Samples

This repository contains Zephyr RTOS demo projects that can run on i.MX devices.

Currently supported board include:

- i.MX 93 11x11 EVK (Cortex-A55)
- i.MX 93 9x9 QSB (Cortex-A55)

## Build and Run

### Setup Zephyr Project

To setup Zephyr project, refer to [Getting Started Guide - Zephyr Project Documentation](https://docs.zephyrproject.org/3.5.0/develop/getting_started/index.html).

Note that when running `west init ~/zephyrproject` command, use the following command instead:

```shell
$ west init -m https://github.com/nxp-imx-support/zephyr --mr v3.5.0-mx93-0 ~/zephyrproject
```

Other steps are the same.

### Build Demo App

To build a demo app, first clone this repository under `~/zephyrproject` :

```shell
$ git clone https://github.com/nxp-imx-support/zephyr_samples
$ cd zephyr_samples
$ git checkout refs/tags/v3.5.0-mx93-0
```

Then build with following commands:

```shell
$ cd ~/zephyrproject/zephyr
$ west build -b $BOARD $PATH_TO_DEMO
```

For example, to build `barcode_scanner` demo, use `west build -b mimx93_evk_a55 ../zephyr_samples/barcode_scanner` .

The result binary is `~/zephyrproject/zephyr/build/zephyr/zephyr.bin` .

### Run Zephyr Demo using SPL Boot

Instead of using u-Boot to load Zephyr binary, SPL boot is used to minimize boot time. To run these demo, [mkimage](https://github.com/nxp-imx/imx-mkimage) tool must be used. To learn how to set up mkimage tool, please refer to chapter 4.5.13 of [i.MX Linux User's Guide](https://www.nxp.com/docs/en/user-guide/IMX_LINUX_USERS_GUIDE.pdf).

Assuming that mkimage tool is set up at `$MKIMAGE` dirctory, and Zephyr build directory is `$BUILD` (`~/zephyrproject/zephyr/build` if following above steps) , run following commands:

```shell
$ cp $BUILD/zephyr/zephyr.bin $MKIMAGE/iMX9/u-boot.bin
$ cd $MKIMAGE
$ make SOC=iMX9 REV=A1 flash_singleboot
```

The mkimage writes bootable binary to `$MKIMAGE/iMX9/flash.bin` . Write the binary to SD card with following command (replace `/dev/sdX` to actual device):

```shell
$ dd if=$MKIMAGE/iMX9/flash.bin of=/dev/sdX bs=1k seek=32 conv=fsync
```

Insert SD card to the board, boot from SD card to run Zephyr demo. The Zephyr shell can be accessed via LPUART1 with setting of `1500000-8-1-0-0` . If connect via on-board debug USB to UART bridge, select 3rd COM/TTY device on the host.

## Demo list

### Barcode Scanner

The barcode_scanner demo runs on a single i.MX 93 11x11 EVK board. It shows preview of MT9M114 parallel camera, scan for QR-Code and shows the info on the LVDS display.

Connect LVDS diaplay to the EVK. Insert `X_RPI_CAM_INT` camera board onto the RPI 20 pin header of the EVK.

Build barcode_scanner using this command:

```shell
$ west build -b mimx93_evk_a55 ../zephyr_samples/barcode_scanner
```

Create bootable SD cand with `mkimage` and `dd`, then boot from it.



### E-bike Digital Cluster

The ebike digital cluster demo include 2 projects: [ebike_digital_cluster](./ebike_digital_cluster) and [ebike_data_source](./ebike_data_source/). It displays a demo instrument panel of an E-bike on an LVDS display, receives status data via CAN network and update the instrument panel.

The ebike_data_source can run on either i.MX 93 11x11 EVK or i.MX 93 9x9 QSB. It  serves as CAN data source, continuously sends simulated status data, while acknowledges latency measurement packets via FlexCAN. The ebike_digital_cluster can only run on i.MX 93 11x11 EVK. It receives information from FlexCAN and displays the data on LVDS display.

Prepare 1x i.MX 93 11x11 EVK board and 1x i.MX 93 9x9 QSB board. Connect CAN interface of the two boards. Connect LVDS display to i.MX 93 11x11 EVK board.

Build ebike_data_source using this command:

```shell
$ west build -b mimx93_qsb_a55 ../zephyr_samples/ebike_data_source
```

Create bootable SD cand with `mkimage` and `dd`, then boot from it. To use another i.MX 93 11x11 EVK as ebike_data_source, replace `mimx93_qsb_a55` to `mimx93_evk_a55` .

Build ebike_ebike_cluster using this command:

```shell
$ west build -b mimx93_evk_a55 ../zephyr_samples/ebike_ebike_cluster
```

Create bootable SD cand with `mkimage` and `dd`, then boot from it.



## License

This repository is licensed under LA_OPT_NXP_Software_License License.




