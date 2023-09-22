import System.Environment

help :: IO ()
help = do
  putStrLn "Please pass the source file path as the only argument"
  putStrLn "Usage: HLint (source file path)"

lexer :: String -> IO ()
lexer filePath = putStrLn $ "Lexer for file: " ++ filePath ++ " is not yet implemented"

parser :: String -> IO ()
parser filePath = putStrLn $ "Parser for file: " ++ filePath ++ " is not yet implemented"

main :: IO ()
main = do
  args <- getArgs
  case args of
    [filePath] -> do
      lexer filePath
      parser filePath
    _ -> help
