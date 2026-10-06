# SheyLanguage

SheyLang adalah bahasa pemrograman tingkat tinggi yang berjalan diatas mesin virtual yang disebut sheylang.

## Struktur File

```file
main.c - Tempat membaca argumen file
lexer.c - Tempat dimana baris kode di pisah dan dikirim ke parser
parser.c - Tempat pengecekan sintaks dan mengirim sinyal kepada lexer untuk berhenti saat error
sheylang.c - Tempat fungsi fungsi dijalankan
```

## Contoh Kode

```sheylang
tampilkan "Hello, World"
tampilkan "And, Hello Delfin"

var int Integer
var string String
var float Float

ubah Integer 1
ubah String "a"
ubah Float 1.0
```

## Hambatan

Bahasa pemrograman ini hanya dibuat untuk eksperimen dan bersenang senang, jadi banyak kekurangan dalam bahasa pemrograman ini
seperti keamanan dan efisiensi. Jadi jangan gunakan bahasa ini untuk kebutuhan produksi atau bisnis.

## Kontribusi

Anda bisa berkontibusi kepada bahasa ini dengan cara fork dan pull request repository ini.

MIT LICENSE
