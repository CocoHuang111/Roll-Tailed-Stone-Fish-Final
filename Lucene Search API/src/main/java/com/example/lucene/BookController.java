package com.example.lucene;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.store.Directory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.multipart.MultipartFile;

@RestController
@RequestMapping("/api/books")
public class BookController {
    private final Directory directory;
    private final Analyzer analyzer;
    private final BookLoader loader;
    private final Path booksDataPath;  // 集中存储路径

    private void ensureDataDirectory() throws IOException {
        if (!Files.exists(booksDataPath.getParent())) {
            Files.createDirectories(booksDataPath.getParent());
        }
    }

    @Autowired
    public BookController(Directory directory, Analyzer analyzer, BookLoader loader,
                        @Value("${app.books.data-path}") String dataPath) {
        this.directory = directory;
        this.analyzer = analyzer;
        this.loader = loader;
        this.booksDataPath = Paths.get(dataPath).toAbsolutePath();
        try {
            ensureDataDirectory();
        } catch (IOException e) {
            throw new RuntimeException("无法创建数据目录: " + booksDataPath.getParent(), e);
        }
    }


    @PostMapping("/init")
    public ResponseEntity<String> initIndex(@RequestParam(required = false) String filePath) throws Exception {
        Path jsonPath = filePath != null ? Paths.get(filePath) : booksDataPath.resolve("books.json");
        
        IndexWriterConfig config = new IndexWriterConfig(analyzer);
        try (IndexWriter writer = new IndexWriter(directory, config)) {
            List<Book> books = loader.loadBooksFromJson(jsonPath.toString());
            new BookIndexer().indexBooks(writer, books);
            return ResponseEntity.ok("索引已更新，共加载 " + books.size() + " 本书");
        }
    }

    @PostMapping("/upload-books")
    public ResponseEntity<String> uploadBooks(
            @RequestParam("file") MultipartFile file) throws Exception {
        
        // 唯一存储位置
        Path targetPath = booksDataPath.resolve("books.json");
        Files.write(targetPath, file.getBytes());
        
        // 自动重建索引
        return initIndex(targetPath.toString());
    }

    // 多字段搜索
    @GetMapping("/search")
    public ResponseEntity<List<Map<String, String>>> search(
            @RequestParam String query,
            @RequestParam(defaultValue = "1") int topN,
            @RequestParam(required = false) Double minPrice,
            @RequestParam(required = false) Double maxPrice) throws Exception {
        
        List<Document> docs;
        Searcher searcher = new Searcher(directory, analyzer);
        if (minPrice != null || maxPrice != null) 
            docs = searcher.search(query, topN, minPrice, maxPrice);
        else
            docs = searcher.search(query, topN);
        
        List<Map<String, String>> resultList = new ArrayList<>();
        for (Document doc : docs) {
        Map<String, String> bookMap = new HashMap<>();
            bookMap.put("title", doc.get("title"));
            bookMap.put("author", doc.get("author"));
            bookMap.put("description", doc.get("description"));
            bookMap.put("price", doc.get("price"));
            // 添加其他需要的字段...
            resultList.add(bookMap);
        }

        return ResponseEntity.ok(resultList);
    }

    // 指定字段搜索
    @PostMapping("/advanced-search")
    public ResponseEntity<List<Map<String, String>>> advancedSearch(
            @RequestBody Map<String, String> fieldQueries,
            @RequestParam(defaultValue = "20") int topN,
            @RequestParam(required = false) Double minPrice,
            @RequestParam(required = false) Double maxPrice) throws Exception {
        
        List<Document> docs;
        Searcher searcher = new Searcher(directory, analyzer);
        if (minPrice != null || maxPrice != null) 
            docs = searcher.search(fieldQueries, topN, minPrice, maxPrice);
        else 
            docs = searcher.search(fieldQueries, topN);

            List<Map<String, String>> resultList = new ArrayList<>();
            for (Document doc : docs) {
            Map<String, String> bookMap = new HashMap<>();
                bookMap.put("title", doc.get("title"));
                bookMap.put("author", doc.get("author"));
                bookMap.put("description", doc.get("description"));
                bookMap.put("price", doc.get("price"));
                bookMap.put("isbn", doc.get("isbn"));
                // 添加其他需要的字段...
                resultList.add(bookMap);
            }

        return ResponseEntity.ok(resultList);
    }
}
