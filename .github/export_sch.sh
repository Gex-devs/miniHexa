

PreviewDir="Preview"

exportSchema()
{
    kicad-cli sch export pdf -b --output $PreviewDir/temp.pdf $1
    pdftoppm -png -singlefile -r 300 $PreviewDir/temp.pdf ./$PreviewDir/$1.png 
}

updateReadme()
{
    # Find the start Scehema section

    # Find End Schema section

    # Read all in between 

    # Check if schematic has been updated

    # Append if it doesn't exit

    # Update if it does
    echo "temp"
}

mkdir -p $PreviewDir # create preview directory if it doesn't exits

for file in $(ls | grep  kicad_sch); do 
    if [ -f "$file" ]; then 
        # schematic files 
        echo "$file" 
        exportSchema $file
        rm $PreviewDir/temp.pdf
    fi 
done





