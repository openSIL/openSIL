# How to update the gh_pages web site content

- Clone openSIL repository

  - >```> git clone <repo> mygh_pages```

- open  (move to) the gh_pages branch

  - > ```> cd  mygh_pages```
    >
    > ```> git checkout gh_pages```

- path to the  Documentation\\APISpec  directory

  - > ```> cd  Documentation\ApiSpec```

- run the Doxygen too for the API spec

  - > ```> GenApiSpec.bat```

  - this creates the API spec html in \docs\API-html

- path to the Documentation\CodeSpec directory

  - > ```> cd  ..\CodeSpec```

- run the Doxygen too for the Code spec

  - > ```> GenCodeSpec.bat```

  - this creates the Code spec html in \docs\Code-html

- Send the updates to the repo
  - ```> cd ..\.. ```
  - ```> git add -f docs\API-html  docs\Code-html```
  - ```> git commit -m "Update the web specs"```
  - ```> git push origin gh_pages```
- -Done-
