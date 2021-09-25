//
//  main.m
//  obelresearch
//
//  Created by Jack Mousseau on 9/24/21.
//

#import <obel/obel.h>

int main(int argc, const char * argv[]) {
    const char *xyz_path = "/Users/jack/Desktop/obel/points.bin";
    const char *ply_path = "/Users/jack/Desktop/obel/points.ply";
    const char *ply_scaled_path = "/Users/jack/Desktop/obel/points_scaled.ply";

    int xyz_fd, ply_fd, ply_scaled_fd;
    if ((xyz_fd = open(xyz_path, O_RDWR)) < 0) { exit(1); }
    if ((ply_fd = open(ply_path, O_RDWR | O_CREAT)) < 0) { exit(1); }
    if ((ply_scaled_fd = open(ply_scaled_path, O_RDWR | O_CREAT)) < 0) { exit(1); }

    obel_cloud_t cloud = { 0 };
    if (obel_cloud_read_bin(xyz_fd, &cloud) < 0) { exit(1); }
    if (obel_cloud_write_ply(ply_fd, &cloud) < 0) { exit(1); }

    for (uint32_t i = 0; i < cloud.count; i++) {
        cloud.points[i] = obel_vector_mul(cloud.points[i], 10.f);
    }

    if (obel_cloud_write_ply(ply_scaled_fd, &cloud) < 0) { exit(1); }
    if (obel_cloud_unmap(&cloud) < 0) { exit(1); }

    close(xyz_fd);
    close(ply_fd);
    close(ply_scaled_fd);

    return 0;
}
