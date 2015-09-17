name = 21crobot
out_dir = $(name)
out_file_dir = $(out_dir)/$(name)
bitbake_out = $(out_dir)/$(name).bb
tarball_fname = $(name).tar.gz
tarball_out = $(out_file_dir)/$(tarball_fname)
license_fname = LICENSE
license_out = $(out_file_dir)/$(license_fname)

ALL: $(bitbake_out) $(tarball_out) $(license_out)
clean:
	rm -f $(bitbake_out) $(tarball_out) $(license_out)
.PHONY: ALL clean

$(tarball_out): ../Framework ../Linux
	mkdir -p $(out_file_dir)
	git archive --format=tar.gz --remote=.. -o $(tarball_out) HEAD Framework Linux

$(license_out): ../LICENSE
	cp ../LICENSE $(license_out)

$(bitbake_out): recipe-header recipe-code $(tarball_out) $(license_out)
	mkdir -p $(out_dir) 
	cp recipe-header $(bitbake_out)
	echo "SRC_URI = \"file://"$(tarball_fname)" file://"$(license_fname)"\"" >> $(bitbake_out)
	echo "SRC_URI[md5sum] = \""$(shell md5sum $(tarball_out) | cut -d' ' -f 1)"\"" >> $(bitbake_out)
	echo "SRC_URI[sha256sum] = \""$(shell sha256sum $(tarball_out) | cut -d' ' -f 1)"\"" >> $(bitbake_out)
	echo "LIC_FILES_CHKSUM = \"file://LICENSE;md5="$(shell md5sum $(license_out) | cut -d' ' -f 1)"\"" >> $(bitbake_out)
	cat recipe-code >> $(bitbake_out)

