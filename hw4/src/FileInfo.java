import java.util.Date;

class FileInfo {
    private String name;
    private long fileSize;
    private String absolutePath;
    private Date lastModifiedTime;

    void setAbsolutePath(String absolutePath) {
        this.absolutePath = absolutePath;
    }

    void setFileSize(long fileSize) {
        this.fileSize = fileSize;
    }

    void setLastModifiedTime(long lastModifiedTime) {
        this.lastModifiedTime = new Date(lastModifiedTime);
    }

    void setName(String name) {
        this.name = name;
    }

    long getFileSize() {
        return fileSize;
    }

    Date getLastModifiedTime() {
        return lastModifiedTime;
    }

    String getAbsolutePath() {
        return absolutePath;
    }

    String getName() {
        return name;
    }
}
