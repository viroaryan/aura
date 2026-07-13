$listener = New-Object System.Net.HttpListener
$listener.Prefixes.Add("http://localhost:8000/")
$listener.Start()
Write-Host "Server started on http://localhost:8000/"

while ($listener.IsListening) {
    $context = $listener.GetContext()
    $response = $context.Response
    
    if (Test-Path "index.html") {
        $content = [System.IO.File]::ReadAllBytes("index.html")
        $response.ContentType = "text/html"
        $response.ContentLength64 = $content.Length
        $response.OutputStream.Write($content, 0, $content.Length)
    } else {
        $response.StatusCode = 404
    }
    $response.Close()
}
