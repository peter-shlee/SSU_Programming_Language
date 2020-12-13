import java.io.File;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

class DirFileList {
    private ArrayList<FileInfo> dirFileList;
    private String[] columnNames = {"Name", "Size", "Path", "Modified Time"};

    DirFileList(String dirPathName, String keyword) throws IllegalPathnameAndKeywordException, NullPointerException, java.util.regex.PatternSyntaxException{
        dirFileList = new ArrayList<FileInfo>();
        createFileList(dirPathName, keyword);
    }

    private void createFileList(String dirPathName, String keyword) throws IllegalPathnameAndKeywordException, NullPointerException, java.util.regex.PatternSyntaxException{
        Path path = Paths.get(dirPathName);
        if (dirPathName.equals("") || keyword.equals("")) throw new IllegalPathnameAndKeywordException("dirPathName or keyword must not be null string"); // 경로명과 검색단어에 아무것도 입력하지 않고 검색 버튼을 누를 경우 예외처리 할 것
        if (!path.isAbsolute()) throw new IllegalPathnameAndKeywordException("dirPathName must be absolute path"); // 경로명은 무조건 절대경로만 입력한다. 절대경로로 입력되지 않을 경우 예외처리 할 것

        if (keyword.indexOf('.') >= 0){ //파일 이름에 확장자 부분에는 '?'를 사용할 수 없다. 확장자 부분에 '?'를 사용할 경우 예외처리 할 것.
            for(int i = keyword.length() - 1; i > 0 ; --i){
                if (keyword.charAt(i) == '.') break;
                if (keyword.charAt(i) == '?') throw new IllegalPathnameAndKeywordException("? must not be in file extension");
            }
        }

        keyword = keyword.replace(".", "\\.");
        keyword = keyword.replace("*", ".*");
        keyword = keyword.replace("?", ".?");
        getDirFileList(dirPathName, keyword); // 메시지 박스
    }

    private void addFileInfoToList(FileInfo newFileInfo){
        dirFileList.add(newFileInfo);
    }

    ArrayList<FileInfo> getDirFileList() {
        ArrayList<FileInfo> copiedDirFileList = new ArrayList<FileInfo>(dirFileList);
        return copiedDirFileList;
    }

    String[] getColumnNames() {
        return columnNames;
    }

    private void getDirFileList(String dirPathName, String keyword) throws NullPointerException, java.util.regex.PatternSyntaxException{ // 입력된 경로명으로부터 하위폴더까지 전부 검색한다.
        for(File file : new File(dirPathName).listFiles()){
            if(file.isDirectory()){
                try {
                    getDirFileList(file.getCanonicalPath().toString(), keyword);
                } catch (IOException e){
                    e.printStackTrace();
                }
            } else if (file.isFile()){
                Pattern pattern = Pattern.compile(keyword);
                Matcher matcher = pattern.matcher(file.getName());
                if(matcher.matches()){
                    FileInfo fileInfo = new FileInfo();
                    fileInfo.setAbsolutePath(file.getAbsolutePath());
                    fileInfo.setFileSize(file.length());
                    fileInfo.setLastModifiedTime(file.lastModified());
                    fileInfo.setName(file.getName());

                    addFileInfoToList(fileInfo);
                }
            }
        }
    }
}